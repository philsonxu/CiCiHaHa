/**
 * @file common.h
 * @brief 数值计算公共头文件：通用数据结构、打印、误差计算、文件IO
 */
#ifndef COMMON_H
#define COMMON_H

#define _GNU_SOURCE
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

#define EPS 1e-10         // 默认精度阈值
#define MAX_ITER 1000    // 默认最大迭代次数

/**
 * @brief 矩阵结构体
 */
typedef struct {
    int rows;
    int cols;
    double **data;
} Matrix;

/**
 * @brief 向量结构体
 */
typedef struct {
    int size;
    double *data;
} Vector;

/* ---------------- 内存分配与释放 ---------------- */
Matrix* mat_create(int rows, int cols);
void    mat_free(Matrix *m);
Vector* vec_create(int size);
void    vec_free(Vector *v);

/* ---------------- 基础运算 ---------------- */
void mat_set(Matrix *m, int i, int j, double val);
double mat_get(const Matrix *m, int i, int j);
void vec_set(Vector *v, int i, double val);
double vec_get(const Vector *v, int i);
Matrix* mat_copy(const Matrix *src);
Vector* vec_copy(const Vector *src);

/* ---------------- 打印函数 ---------------- */
void mat_print(const char *name, const Matrix *m, int precision);
void vec_print(const char *name, const Vector *v, int precision);
void print_result(const char *title, double computed, double expected, double eps);

/* ---------------- 误差与验证 ---------------- */
double vec_norm2(const Vector *v);
double vec_error_l2(const Vector *computed, const Vector *expected);
double mat_error_fro(const Matrix *computed, const Matrix *expected);
int    verify_double(double computed, double expected, double eps);
int    verify_vector(const Vector *computed, const Vector *expected, double eps);
int    verify_matrix(const Matrix *computed, const Matrix *expected, double eps);

/* ---------------- 文件IO：读写测试数据 ---------------- */
Matrix* mat_read(const char *filename);
Vector* vec_read(const char *filename);
int     mat_write(const char *filename, const Matrix *m);
int     vec_write(const char *filename, const Vector *v);

/* ---------------- 计时工具 ---------------- */
double get_time_sec();

#endif // COMMON_H
