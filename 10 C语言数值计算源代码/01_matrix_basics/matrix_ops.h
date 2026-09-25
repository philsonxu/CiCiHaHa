/**
 * @file matrix_ops.h
 * @brief 矩阵基础运算：加减乘、转置、行列式、LU分解、矩阵求逆
 */
#ifndef MATRIX_OPS_H
#define MATRIX_OPS_H

#include "../common/common.h"

Matrix* mat_add(const Matrix *a, const Matrix *b);
Matrix* mat_sub(const Matrix *a, const Matrix *b);
Matrix* mat_mul(const Matrix *a, const Matrix *b);
Matrix* mat_transpose(const Matrix *a);
double  mat_det(const Matrix *a);

/**
 * @brief LU分解（带部分选主元） PA = LU
 * @param L 输出：单位下三角矩阵
 * @param U 输出：上三角矩阵
 * @param P 输出：置换矩阵
 * @return 0=成功，-1=奇异矩阵
 */
int mat_lu_decomp(const Matrix *A, Matrix *L, Matrix *U, Matrix *P);

/**
 * @brief 矩阵求逆（基于LU分解）
 * @return 逆矩阵，若A奇异返回NULL
 */
Matrix* mat_inverse(const Matrix *A);

#endif
