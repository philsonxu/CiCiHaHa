#include "cllm.h"

// ========== 随机数 (PCG-XSH-RR 风格，简单快速) ==========
static uint64_t rng_state = 0x123456789ABCDEF0ULL;
static uint64_t rng_inc   = 0xDA3E39CB94B95BDBULL;

void cllm_rand_seed(uint64_t seed) {
    rng_state = seed + rng_inc;
    // warm up
    cllm_rand_uniform(0.0f, 1.0f);
}

float cllm_rand_uniform(float a, float b) {
    uint64_t old = rng_state;
    rng_state = old * 6364136223846793005ULL + (rng_inc | 1);
    uint32_t xorshifted = (uint32_t)(((old >> 18) ^ old) >> 27);
    uint32_t rot = (uint32_t)(old >> 59);
    uint32_t r = (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
    float u = (r >> 8) * 0x1.0p-24f; // [0,1)
    return a + (b - a) * u;
}

// Box-Muller 高斯
float cllm_rand_normal(float mean, float stddev) {
    float u1 = cllm_rand_uniform(1e-7f, 1.0f);
    float u2 = cllm_rand_uniform(0.0f, 1.0f);
    float z = sqrtf(-2.0f * logf(u1)) * cosf(2.0f * (float)M_PI * u2);
    return mean + stddev * z;
}

// ========== Matrix ==========
Matrix* matrix_create(int rows, int cols) {
    assert(rows > 0 && cols > 0);
    Matrix *m = (Matrix*)calloc(1, sizeof(Matrix));
    if (!m) return NULL;
    m->rows = rows; m->cols = cols;
    m->data = (float*)calloc(rows * cols, sizeof(float));
    m->grad = (float*)calloc(rows * cols, sizeof(float));
    if (!m->data || !m->grad) { free(m->data); free(m->grad); free(m); return NULL; }
    return m;
}

void matrix_free(Matrix *m) {
    if (!m) return;
    free(m->data); free(m->grad); free(m);
}

void matrix_fill(Matrix *m, float v) {
    for (int i = 0; i < m->rows * m->cols; i++) m->data[i] = v;
}

void matrix_randn(Matrix *m, float stddev) {
    for (int i = 0; i < m->rows * m->cols; i++)
        m->data[i] = cllm_rand_normal(0.0f, stddev);
}

void matrix_zeros_like(Matrix *m) {
    memset(m->data, 0, m->rows * m->cols * sizeof(float));
    memset(m->grad, 0, m->rows * m->cols * sizeof(float));
}

void matrix_copy(Matrix *dst, const Matrix *src) {
    assert(dst->rows == src->rows && dst->cols == src->cols);
    memcpy(dst->data, src->data, src->rows * src->cols * sizeof(float));
}

// ========== Tensor3D ==========
Tensor3D* tensor3d_create(int B, int T, int C) {
    assert(B>0 && T>0 && C>0);
    Tensor3D *t = (Tensor3D*)calloc(1, sizeof(Tensor3D));
    if (!t) return NULL;
    t->B = B; t->T = T; t->C = C;
    size_t n = (size_t)B * T * C;
    t->data = (float*)calloc(n, sizeof(float));
    t->grad = (float*)calloc(n, sizeof(float));
    if (!t->data || !t->grad) { free(t->data); free(t->grad); free(t); return NULL; }
    return t;
}

void tensor3d_free(Tensor3D *t) {
    if (!t) return;
    free(t->data); free(t->grad); free(t);
}

void tensor3d_fill(Tensor3D *t, float v) {
    size_t n = (size_t)t->B * t->T * t->C;
    for (size_t i = 0; i < n; i++) t->data[i] = v;
}

void tensor3d_zeros(Tensor3D *t) {
    size_t n = (size_t)t->B * t->T * t->C;
    memset(t->data, 0, n * sizeof(float));
    memset(t->grad, 0, n * sizeof(float));
}

// ========== 矩阵乘法（朴素但正确，i-k-j 顺序缓存友好） ==========
void matmul(float *C, const float *A, const float *B, int M, int K, int N) {
    for (int i = 0; i < M; i++) {
        for (int k = 0; k < K; k++) {
            float a = A[i*K + k];
            const float *Bk = B + k*N;
            float *Ci = C + i*N;
            #pragma GCC ivdep
            for (int j = 0; j < N; j++) {
                Ci[j] += a * Bk[j];
            }
        }
    }
}

// C[M,N] = A^T[K,M] @ B[K,N]  -> C[i,j] = sum_k A[k,i]*B[k,j]
void matmul_AT_B(float *C, const float *A, const float *B, int M, int K, int N) {
    for (int i = 0; i < M; i++) {
        for (int k = 0; k < K; k++) {
            float a = A[k*M + i];
            const float *Bk = B + k*N;
            float *Ci = C + i*N;
            for (int j = 0; j < N; j++) Ci[j] += a * Bk[j];
        }
    }
}

// C[M,N] = A[M,K] @ B^T[N,K]  -> C[i,j] = sum_k A[i,k]*B[j,k]
void matmul_A_BT(float *C, const float *A, const float *B, int M, int K, int N) {
    for (int i = 0; i < M; i++) {
        const float *Ai = A + i*K;
        float *Ci = C + i*N;
        for (int j = 0; j < N; j++) {
            const float *Bj = B + j*K;
            float s = 0.0f;
            for (int k = 0; k < K; k++) s += Ai[k] * Bj[k];
            Ci[j] += s;
        }
    }
}

void vec_add(float *out, const float *a, const float *b, int n) {
    for (int i = 0; i < n; i++) out[i] = a[i] + b[i];
}

void vec_scale(float *out, const float *a, float s, int n) {
    for (int i = 0; i < n; i++) out[i] = a[i] * s;
}

void vec_add_scaled(float *out, const float *a, float s, int n) {
    for (int i = 0; i < n; i++) out[i] += s * a[i];
}
