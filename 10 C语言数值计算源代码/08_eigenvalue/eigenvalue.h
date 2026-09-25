/**
 * @file eigenvalue.h
 * @brief 特征值计算：幂法（最大模特征值）、反幂法（最小模）、Jacobi旋转法（对称矩阵全部特征值）
 */
#ifndef EIGENVALUE_H
#define EIGENVALUE_H

#include "../common/common.h"

/**
 * @brief 幂法求矩阵按模最大特征值与特征向量
 * @param[out] eigenvec 输出特征向量（需已分配，大小n）
 * @return 特征值
 */
double power_iteration(const Matrix *A, double *eigenvec, double tol, int max_iter, int *iter);

/** @brief 反幂法求按模最小特征值 */
double inverse_iteration(const Matrix *A, double *eigenvec, double tol, int max_iter, int *iter);

/**
 * @brief Jacobi旋转法求实对称矩阵全部特征值与特征向量
 * @param eigenvalues 输出特征值数组（已分配，大小n）
 * @param eigenvectors 输出特征向量矩阵（列向量，已创建n×n）
 */
void jacobi_eigen(const Matrix *A, double *eigenvalues, Matrix *eigenvectors, double tol);

#endif
