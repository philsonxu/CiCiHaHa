/**
 * @file test_ode.c
 * @brief ODE求解验证：y'=-y / y'=y / y'=cos(t)，4种算法对比误差与步数
 */
#define _GNU_SOURCE
#include "ode_solver.h"
#include <math.h>

static double f_decay(double t, double y) { (void)t; return -y; }
static double f_growth(double t, double y){ (void)t; return y; }
static double f_sin(double t, double y)   { (void)y; return cos(t); }

static double exact_decay(double t) { return exp(-t); }
static double exact_growth(double t){ return exp(t); }
static double exact_sin(double t)   { return sin(t); }

#define PR(name, r, ev) printf("  %-12s 步数=%4d  终点y=%.10f  误差=%.2e\n", \
                               name, (r)->n-1, (r)->y[(r)->n-1], fabs((r)->y[(r)->n-1]-(ev)))

static void run_compare(const char *name, ODEFunc f, double y0, double t_end,
                        double (*exact)(double), double h) {
    printf("\n--- %s (t_end=%.1f, h=%.3f) 精确值y(%.1f)=%.10f ---\n",
           name, t_end, h, t_end, exact(t_end));
    ODEResult *re = euler(f, 0, y0, t_end, h);
    ODEResult *ri = euler_improved(f, 0, y0, t_end, h);
    ODEResult *rk = rk4(f, 0, y0, t_end, h);
    ODEResult *ra = rk45(f, 0, y0, t_end, 1e-8, h/10);
    double ev = exact(t_end);
    PR("显式欧拉", re, ev);
    PR("改进欧拉", ri, ev);
    PR("RK4", rk, ev);
    PR("自适应RK45", ra, ev);
    ode_result_free(re); ode_result_free(ri); ode_result_free(rk); ode_result_free(ra);
}
#undef PR

int main() {
    printf("\n========== ODE初值问题求解验证 ==========\n");
    run_compare("y'=-y, y(0)=1, 解y=e^{-t}",       f_decay,  1.0, 5.0, exact_decay,  0.1);
    run_compare("y'=y,  y(0)=1, 解y=e^t",          f_growth, 1.0, 2.0, exact_growth, 0.05);
    run_compare("y'=cos(t), y(0)=0, 解y=sin(t)",   f_sin,    0.0, M_PI, exact_sin,  0.1);
    printf("\n========== ODE求解全部验证完成 ==========\n");
    return 0;
}
