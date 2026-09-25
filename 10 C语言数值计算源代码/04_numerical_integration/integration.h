/**
 * @file integration.h
 * @brief 一维数值积分
 *  方法：复合梯形法、复合辛普森法、自适应辛普森、龙贝格(Romberg)积分、高斯-勒让德求积
 */
#ifndef INTEGRATION_H
#define INTEGRATION_H

#include "../common/common.h"

typedef double (*Func1D)(double x);

double trapezoidal(Func1D f, double a, double b, int n);
double simpson(Func1D f, double a, double b, int n);
double adaptive_simpson(Func1D f, double a, double b, double tol);
double romberg(Func1D f, double a, double b, double tol, int max_level);

/**
 * @brief 高斯-勒让德n点求积（n=2,3,4,5,8,16内置权重与节点）
 */
double gauss_legendre(Func1D f, double a, double b, int n);

#endif
