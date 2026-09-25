/**
 * @file ode_solver.h
 * @brief 常微分方程初值问题 y'=f(t,y), y(t0)=y0
 *  方法：显式欧拉、改进欧拉（Heun）、经典RK4、自适应RK45(Dormand-Prince)
 */
#ifndef ODE_SOLVER_H
#define ODE_SOLVER_H

#include "../common/common.h"

typedef double (*ODEFunc)(double t, double y);

typedef struct {
    int n;           // 输出步数
    double *t;       // 时间点数组
    double *y;       // 数值解数组
} ODEResult;

ODEResult* ode_result_create(int n);
void       ode_result_free(ODEResult *r);

/** @brief 显式欧拉法 */
ODEResult* euler(ODEFunc f, double t0, double y0, double t_end, double h);

/** @brief 改进欧拉（Heun预测-校正） */
ODEResult* euler_improved(ODEFunc f, double t0, double y0, double t_end, double h);

/** @brief 经典4阶Runge-Kutta */
ODEResult* rk4(ODEFunc f, double t0, double y0, double t_end, double h);

/**
 * @brief 自适应RK45(Dormand-Prince pair)
 * @param tol 每步局部误差容限
 */
ODEResult* rk45(ODEFunc f, double t0, double y0, double t_end, double tol, double h_init);

#endif
