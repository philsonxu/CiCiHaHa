/**
 * @file interpolation.h
 * @brief 插值算法：拉格朗日插值、牛顿差商插值、三次样条插值（自然边界）
 */
#ifndef INTERPOLATION_H
#define INTERPOLATION_H

#include "../common/common.h"

typedef struct {
    int n;          // 节点数
    double *x;
    double *y;
} PolyInterp;

PolyInterp* poly_interp_create(const double *xs, const double *ys, int n);
void        poly_interp_free(PolyInterp *p);
double      lagrange_eval(const PolyInterp *p, double x);
double      newton_eval(const PolyInterp *p, double x); // 内部预计算差商

typedef struct {
    int n;
    double *x, *y;
    double *a, *b, *c, *d; // 三次样条系数 S_i(x)=a+b(x-xi)+c(x-xi)^2+d(x-xi)^3
} Spline;

Spline* spline_create(const double *xs, const double *ys, int n);
void    spline_free(Spline *s);
double  spline_eval(const Spline *s, double x);

#endif
