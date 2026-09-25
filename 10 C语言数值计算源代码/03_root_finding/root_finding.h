/**
 * @file root_finding.h
 * @brief 非线性方程 f(x)=0 求根
 *  方法：二分法、牛顿法、割线法（弦截法）、不动点迭代
 */
#ifndef ROOT_FINDING_H
#define ROOT_FINDING_H

#include "../common/common.h"

typedef double (*Func1D)(double x);
typedef double (*Func1DD)(double x); // 导数

/** @brief 二分法，返回根，*iter返回迭代次数 */
double bisection(Func1D f, double a, double b, double tol, int max_iter, int *iter);

/** @brief 牛顿法 */
double newton_method(Func1D f, Func1DD df, double x0, double tol, int max_iter, int *iter);

/** @brief 割线法 */
double secant_method(Func1D f, double x0, double x1, double tol, int max_iter, int *iter);

/** @brief 不动点迭代 x_{k+1}=g(x_k) */
double fixed_point(Func1D g, double x0, double tol, int max_iter, int *iter);

#endif
