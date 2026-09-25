/**
 * @file linear_solver.h
 * @brief 线性方程组 Ax=b 求解器
 *  方法：高斯消元法（部分选主元）、雅可比迭代、高斯-赛德尔迭代、共轭梯度法(CG)
 */
#ifndef LINEAR_SOLVER_H
#define LINEAR_SOLVER_H

#include "../common/common.h"

/** @brief 高斯消元法直接求解 Ax=b */
int gauss_solve(const Matrix *A, const Vector *b, Vector *x);

/**
 * @brief 雅可比迭代法
 * @param tol 收敛容差，返回实际迭代次数
 */
int jacobi_solve(const Matrix *A, const Vector *b, Vector *x, double tol, int max_iter);

/** @brief 高斯-赛德尔迭代法 */
int gauss_seidel_solve(const Matrix *A, const Vector *b, Vector *x, double tol, int max_iter);

/**
 * @brief 共轭梯度法（适用于对称正定矩阵）
 * @return 迭代次数
 */
int cg_solve(const Matrix *A, const Vector *b, Vector *x, double tol, int max_iter);

#endif
