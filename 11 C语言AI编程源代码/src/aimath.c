/**
 * @file aimath.c - 核心数学库实现
 */
#include "aimath.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <assert.h>

/* ======== 随机数 ======== */
static unsigned int _seed = 42;
void random_seed(unsigned int seed) { _seed = seed; srand(seed); }

/* 简单LCG均匀随机, 跨平台一致性比rand()好 */
float random_uniform(float lo, float hi) {
    _seed = _seed * 1103515245u + 12345u;
    float r = ((_seed >> 16) & 0x7fff) / 32767.0f;
    return lo + r * (hi - lo);
}

/* Box-Muller变换: 标准正态 */
float random_normal(float mu, float sigma) {
    float u1 = random_uniform(1e-7f, 1.0f);
    float u2 = random_uniform(0.0f, 1.0f);
    float z = sqrtf(-2.0f * logf(u1)) * cosf(2.0f * 3.14159265358979f * u2);
    return mu + sigma * z;
}

void init_xavier(Matrix* w, size_t fan_in, size_t fan_out) {
    float limit = sqrtf(6.0f / (fan_in + fan_out));
    for (size_t i = 0; i < w->rows * w->cols; i++)
        w->data[i] = random_uniform(-limit, limit);
}

void init_he(Matrix* w, size_t fan_in) {
    float std = sqrtf(2.0f / fan_in);
    for (size_t i = 0; i < w->rows * w->cols; i++)
        w->data[i] = random_normal(0.0f, std);
}

/* ======== 向量 ======== */
void vec_add(const float* a, const float* b, float* out, size_t n) {
    for (size_t i = 0; i < n; i++) out[i] = a[i] + b[i];
}
void vec_sub(const float* a, const float* b, float* out, size_t n) {
    for (size_t i = 0; i < n; i++) out[i] = a[i] - b[i];
}
void vec_mul_scalar(const float* a, float s, float* out, size_t n) {
    for (size_t i = 0; i < n; i++) out[i] = a[i] * s;
}
float vec_dot(const float* a, const float* b, size_t n) {
    float s = 0; for (size_t i = 0; i < n; i++) s += a[i] * b[i]; return s;
}
float vec_sum(const float* a, size_t n) {
    float s = 0; for (size_t i = 0; i < n; i++) s += a[i]; return s;
}
float vec_mean(const float* a, size_t n) { return vec_sum(a,n)/(float)n; }
float vec_l2_norm(const float* a, size_t n) { return sqrtf(vec_dot(a,a,n)); }
void vec_fill(float* a, float v, size_t n) { for (size_t i = 0; i < n; i++) a[i] = v; }
void vec_copy(const float* src, float* dst, size_t n) { memcpy(dst, src, n*sizeof(float)); }
float vec_max(const float* a, size_t n, size_t* max_idx) {
    float mv = a[0]; size_t mi = 0;
    for (size_t i = 1; i < n; i++) if (a[i] > mv) { mv = a[i]; mi = i; }
    if (max_idx) *max_idx = mi;
    return mv;
}

/* ======== 矩阵 ======== */
Matrix mat_create(size_t rows, size_t cols) {
    Matrix m; m.rows = rows; m.cols = cols;
    m.data = (float*)calloc(rows*cols, sizeof(float));
    return m;
}
void mat_free(Matrix* m) { if (m->data) free(m->data); m->data = NULL; m->rows=m->cols=0; }
Matrix mat_clone(const Matrix* m) {
    Matrix c = mat_create(m->rows, m->cols);
    memcpy(c.data, m->data, m->rows*m->cols*sizeof(float));
    return c;
}
void mat_fill(Matrix* m, float v) { vec_fill(m->data, v, m->rows*m->cols); }
void mat_set_identity(Matrix* m) {
    mat_fill(m, 0);
    for (size_t i = 0; i < m->rows && i < m->cols; i++)
        *mat_at(m, i, i) = 1.0f;
}
void mat_copy(const Matrix* src, Matrix* dst) {
    assert(src->rows == dst->rows && src->cols == dst->cols);
    memcpy(dst->data, src->data, src->rows*src->cols*sizeof(float));
}
float* mat_at(Matrix* m, size_t i, size_t j) { return &m->data[i*m->cols + j]; }
const float* mat_at_c(const Matrix* m, size_t i, size_t j) { return &m->data[i*m->cols + j]; }

void mat_mul(const Matrix* a, const Matrix* b, Matrix* c) {
    assert(a->cols == b->rows);
    assert(c->rows == a->rows && c->cols == b->cols);
    size_t m = a->rows, k = a->cols, n = b->cols;
    mat_fill(c, 0);
    for (size_t i = 0; i < m; i++) {
        for (size_t p = 0; p < k; p++) {
            float aip = *mat_at_c(a, i, p);
            if (fabsf(aip) < 1e-12f) continue;
            const float* brow = &b->data[p*n];
            float* crow = &c->data[i*n];
            for (size_t j = 0; j < n; j++) crow[j] += aip * brow[j];
        }
    }
}

void mat_transpose(const Matrix* a, Matrix* at) {
    assert(at->rows == a->cols && at->cols == a->rows);
    for (size_t i = 0; i < a->rows; i++)
        for (size_t j = 0; j < a->cols; j++)
            *mat_at(at, j, i) = *mat_at_c(a, i, j);
}

void mat_mul_add(const Matrix* a, const Matrix* b, Matrix* c) {
    assert(a->cols == b->rows && c->rows == a->rows && c->cols == b->cols);
    size_t m = a->rows, k = a->cols, n = b->cols;
    for (size_t i = 0; i < m; i++)
        for (size_t p = 0; p < k; p++) {
            float aip = *mat_at_c(a,i,p);
            if (fabsf(aip) < 1e-12f) continue;
            for (size_t j = 0; j < n; j++)
                *mat_at(c,i,j) += aip * *mat_at_c(b,p,j);
        }
}

void mat_transpose_mul(const Matrix* a, const Matrix* b, Matrix* c) {
    assert(a->rows == b->rows && c->rows == a->cols && c->cols == b->cols);
    size_t m = a->rows, k = a->cols, n = b->cols;
    mat_fill(c, 0);
    for (size_t i = 0; i < m; i++) {
        const float* ar = &a->data[i*k];
        const float* br = &b->data[i*n];
        for (size_t p = 0; p < k; p++) {
            float ap = ar[p];
            for (size_t j = 0; j < n; j++)
                *mat_at(c,p,j) += ap * br[j];
        }
    }
}

void mat_mul_transpose(const Matrix* a, const Matrix* b, Matrix* c) {
    /* C = A * B^T
     * A: m x k, B: n x k, C: m x n
     * C[i,j] = sum_p A[i,p] * B[j,p] */
    assert(a->cols == b->cols && c->rows == a->rows && c->cols == b->rows);
    size_t m = a->rows, k = a->cols, n = b->rows;
    mat_fill(c,0);
    for (size_t i=0;i<m;i++) {
        const float* ar = &a->data[i*k];
        for (size_t j=0;j<n;j++) {
            const float* br = &b->data[j*k];
            float s=0;
            for (size_t p=0;p<k;p++) s += ar[p]*br[p];
            *mat_at(c,i,j) += s;
        }
    }
}

void mat_row_add(Matrix* m, size_t i, const float* v) {
    for (size_t j = 0; j < m->cols; j++) *mat_at(m,i,j) += v[j];
}
void mat_col_add(Matrix* m, size_t j, const float* v) {
    for (size_t i = 0; i < m->rows; i++) *mat_at(m,i,j) += v[i];
}
void mat_scale(Matrix* m, float s) {
    for (size_t i=0; i<m->rows*m->cols; i++) m->data[i] *= s;
}
void mat_add_scalar(Matrix* m, float s) {
    for (size_t i=0; i<m->rows*m->cols; i++) m->data[i] += s;
}
void mat_apply(const Matrix* m, Matrix* out, float (*f)(float)) {
    assert(m->rows==out->rows && m->cols==out->cols);
    for (size_t i=0;i<m->rows*m->cols;i++) out->data[i] = f(m->data[i]);
}
void mat_apply_inplace(Matrix* m, float (*f)(float)) {
    for (size_t i=0;i<m->rows*m->cols;i++) m->data[i] = f(m->data[i]);
}

/* ======== 激活函数 ======== */
float sigmoid(float x) { return 1.0f/(1.0f + expf(-x)); }
float sigmoid_deriv(float s) { return s*(1-s); }
float relu(float x) { return x > 0 ? x : 0; }
float relu_deriv(float x) { return x > 0 ? 1.0f : 0; }
float relu6(float x) { float r=x; if (r<0)r=0; if (r>6)r=6; return r; }
float tanh_act(float x) { return tanhf(x); }
float tanh_deriv(float t) { return 1 - t*t; }
float leaky_relu(float x) { return x > 0 ? x : 0.01f*x; }
float leaky_relu_deriv(float x) { return x > 0 ? 1.0f : 0.01f; }
float softplus(float x) { return logf(1+expf(x)); }

void softmax_row(float* row, size_t n) {
    float m = row[0]; for (size_t i=1;i<n;i++) if (row[i]>m) m=row[i];
    float s = 0; for (size_t i=0;i<n;i++) { row[i] = expf(row[i]-m); s += row[i]; }
    for (size_t i=0;i<n;i++) row[i] /= s;
}
