#ifndef CLLM_H
#define CLLM_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <assert.h>

// ========== 错误与宏 ==========
#define CLLM_OK           0
#define CLLM_ERR_NULL    -1
#define CLLM_ERR_SHAPE   -2
#define CLLM_ERR_FILE    -3
#define CLLM_ERR_MEM     -4

#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b))

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// 随机数
float cllm_rand_uniform(float a, float b);
void  cllm_rand_seed(uint64_t seed);
float cllm_rand_normal(float mean, float stddev);  // Box-Muller

// ========== 张量 ==========
// 2D 矩阵 (rows x cols)，用于权重
typedef struct {
    int rows;
    int cols;
    float *data;
    float *grad;
} Matrix;

Matrix* matrix_create(int rows, int cols);
void    matrix_free(Matrix *m);
void    matrix_fill(Matrix *m, float v);
void    matrix_randn(Matrix *m, float stddev);
void    matrix_zeros_like(Matrix *m);
void    matrix_copy(Matrix *dst, const Matrix *src);

// 3D 激活张量 (batch x seq x dim)，用于激活值和残差流
typedef struct {
    int B;  // batch
    int T;  // time / seq_len
    int C;  // channels / dim
    float *data;
    float *grad;
} Tensor3D;

Tensor3D* tensor3d_create(int B, int T, int C);
void      tensor3d_free(Tensor3D *t);
void      tensor3d_fill(Tensor3D *t, float v);
void      tensor3d_zeros(Tensor3D *t);

// 行/切片访问宏
#define MAT_AT(m, i, j)          (m).data[(i)*(m).cols + (j)]
#define MAT_GRAD_AT(m, i, j)     (m).grad[(i)*(m).cols + (j)]
#define T3D_AT(t, b, t_idx, c)   (t).data[((b)*(t).T + (t_idx))*(t).C + (c)]
#define T3D_GRAD_AT(t, b, t_idx, c) (t).grad[((b)*(t).T + (t_idx))*(t).C + (c)]

// ========== 矩阵运算 ==========
// C[M,N] = A[M,K] @ B[K,N]
void matmul(float *C, const float *A, const float *B, int M, int K, int N);
// C[M,N] = A^T[K,M] @ B[K,N]  (用于反向传播 dL/dW)
void matmul_AT_B(float *C, const float *A, const float *B, int M, int K, int N);
// C[M,N] = A[M,K] @ B^T[N,K]  (用于反向传播 dL/dx)
void matmul_A_BT(float *C, const float *A, const float *B, int M, int K, int N);

// ========== 逐元素运算 ==========
void vec_add(float *out, const float *a, const float *b, int n);
void vec_scale(float *out, const float *a, float s, int n);
void vec_add_scaled(float *out, const float *a, float s, int n); // out += s*a

#endif
