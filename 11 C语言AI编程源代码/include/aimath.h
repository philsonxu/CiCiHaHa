/**
 * @file aimath.h
 * @brief C语言AI核心数学库 - 向量/矩阵运算
 */
#ifndef AIMATH_H
#define AIMATH_H

#include <stddef.h>
#include <stdbool.h>

#define EPS 1e-8f
#ifndef MAX
#define MAX(a,b) ((a)>(b)?(a):(b))
#endif
#ifndef max
#define max(a,b) MAX(a,b)
#endif

/* ========== 向量运算 ========== */
void vec_add(const float* a, const float* b, float* out, size_t n);
void vec_sub(const float* a, const float* b, float* out, size_t n);
void vec_mul_scalar(const float* a, float s, float* out, size_t n);
float vec_dot(const float* a, const float* b, size_t n);
float vec_sum(const float* a, size_t n);
float vec_mean(const float* a, size_t n);
float vec_l2_norm(const float* a, size_t n);
void vec_fill(float* a, float v, size_t n);
void vec_copy(const float* src, float* dst, size_t n);
float vec_max(const float* a, size_t n, size_t* max_idx);

/* ========== 矩阵运算 ========== */
/**
 * Matrix 存储: m行n列, data[i*n + j] = M[i][j], 行优先存储
 */
typedef struct {
    size_t rows, cols;
    float* data;
} Matrix;

Matrix mat_create(size_t rows, size_t cols);
void mat_free(Matrix* m);
Matrix mat_clone(const Matrix* m);
void mat_fill(Matrix* m, float v);
void mat_set_identity(Matrix* m);
void mat_copy(const Matrix* src, Matrix* dst);

float* mat_at(Matrix* m, size_t i, size_t j);
const float* mat_at_c(const Matrix* m, size_t i, size_t j);

/* C = A * B */
void mat_mul(const Matrix* a, const Matrix* b, Matrix* c);
/* C = A^T */
void mat_transpose(const Matrix* a, Matrix* at);
/* C += A * B */
void mat_mul_add(const Matrix* a, const Matrix* b, Matrix* c);
/* C += A^T * B */
void mat_transpose_mul(const Matrix* a, const Matrix* b, Matrix* c);
/* C += A * B^T */
void mat_mul_transpose(const Matrix* a, const Matrix* b, Matrix* c);

/* 行/列操作 */
void mat_row_add(Matrix* m, size_t i, const float* v);
void mat_col_add(Matrix* m, size_t j, const float* v);
void mat_scale(Matrix* m, float s);
void mat_add_scalar(Matrix* m, float s);
void mat_apply(const Matrix* m, Matrix* out, float (*f)(float));
void mat_apply_inplace(Matrix* m, float (*f)(float));

/* ========== 随机数与初始化 ========== */
void random_seed(unsigned int seed);
float random_uniform(float lo, float hi);
float random_normal(float mu, float sigma);
/* Xavier均匀初始化: [-sqrt(6/(fan_in+fan_out)), sqrt(6/(fan_in+fan_out))] */
void init_xavier(Matrix* w, size_t fan_in, size_t fan_out);
/* He初始化: N(0, sqrt(2/fan_in)), ReLU推荐 */
void init_he(Matrix* w, size_t fan_in);

/* ========== 激活函数 ========== */
float sigmoid(float x);
float sigmoid_deriv(float x);  /* 输入是sigmoid(x) */
float relu(float x);
float relu_deriv(float x);    /* 输入是x */
float relu6(float x);
float tanh_act(float x);
float tanh_deriv(float x);    /* 输入是tanh(x) */
float leaky_relu(float x);
float leaky_relu_deriv(float x);
float softplus(float x);

/* Softmax 作用在一行上 */
void softmax_row(float* row, size_t n);

#endif
