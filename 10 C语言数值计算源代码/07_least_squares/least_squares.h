/**
 * @file least_squares.h
 * @brief 最小二乘拟合：线性最小二乘、多项式拟合
 */
#ifndef LEAST_SQUARES_H
#define LEAST_SQUARES_H

#include "../common/common.h"

/**
 * @brief 多项式拟合 y = c0 + c1*x + c2*x^2 + ... + cm*x^m
 * @param x, y 数据点
 * @param n 数据点数
 * @param m 多项式阶数（m+1个系数）
 * @param coeff 输出系数数组（已分配，长度m+1）
 */
void poly_fit(const double *x, const double *y, int n, int m, double *coeff);

/**
 * @brief 计算拟合后的y值
 */
double poly_eval(const double *coeff, int m, double x);

/**
 * @brief 计算R²决定系数
 */
double r_squared(const double *x, const double *y, int n, const double *coeff, int m);

#endif
