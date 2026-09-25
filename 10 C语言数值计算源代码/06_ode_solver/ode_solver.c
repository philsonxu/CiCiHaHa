/**
 * @file ode_solver.c
 * @brief ODE初值问题求解实现
 */
#include "ode_solver.h"
#include <stdlib.h>

ODEResult* ode_result_create(int n) {
    ODEResult *r = (ODEResult*)malloc(sizeof(ODEResult));
    r->n = n;
    r->t = (double*)malloc(n*sizeof(double));
    r->y = (double*)malloc(n*sizeof(double));
    return r;
}

void ode_result_free(ODEResult *r) {
    if (!r) return;
    free(r->t); free(r->y); free(r);
}

ODEResult* euler(ODEFunc f, double t0, double y0, double t_end, double h) {
    int n = (int)((t_end - t0)/h) + 1;
    ODEResult *r = ode_result_create(n);
    double t = t0, y = y0;
    r->t[0] = t; r->y[0] = y;
    for (int i = 1; i < n; i++) {
        y = y + h * f(t, y);
        t = t0 + i*h;
        r->t[i] = t; r->y[i] = y;
    }
    return r;
}

ODEResult* euler_improved(ODEFunc f, double t0, double y0, double t_end, double h) {
    int n = (int)((t_end - t0)/h) + 1;
    ODEResult *r = ode_result_create(n);
    double t = t0, y = y0;
    r->t[0] = t; r->y[0] = y;
    for (int i = 1; i < n; i++) {
        double k1 = f(t, y);
        double k2 = f(t + h, y + h*k1);
        y = y + h*(k1 + k2)/2.0;
        t = t0 + i*h;
        r->t[i] = t; r->y[i] = y;
    }
    return r;
}

ODEResult* rk4(ODEFunc f, double t0, double y0, double t_end, double h) {
    int n = (int)((t_end - t0)/h) + 1;
    ODEResult *r = ode_result_create(n);
    double t = t0, y = y0;
    r->t[0] = t; r->y[0] = y;
    for (int i = 1; i < n; i++) {
        double k1 = f(t, y);
        double k2 = f(t + h/2, y + h*k1/2);
        double k3 = f(t + h/2, y + h*k2/2);
        double k4 = f(t + h,   y + h*k3);
        y = y + h*(k1 + 2*k2 + 2*k3 + k4)/6.0;
        t = t0 + i*h;
        r->t[i] = t; r->y[i] = y;
    }
    return r;
}

/** 动态数组辅助 */
typedef struct { double *t, *y; int size, cap; } DynArr;
static void dyn_push(DynArr *d, double t, double y) {
    if (d->size >= d->cap) { d->cap = d->cap?d->cap*2:64; d->t=realloc(d->t,d->cap*sizeof(double)); d->y=realloc(d->y,d->cap*sizeof(double));}
    d->t[d->size]=t; d->y[d->size]=y; d->size++;
}

ODEResult* rk45(ODEFunc f, double t0, double y0, double t_end, double tol, double h_init) {
    // Dormand-Prince RK4(5) Butcher表
    static const double c2=1.0/5, c3=3.0/10, c4=4.0/5, c5=8.0/9, c6=1.0, c7=1.0;
    static const double a21=1.0/5;
    static const double a31=3.0/40, a32=9.0/40;
    static const double a41=44.0/45, a42=-56.0/15, a43=32.0/9;
    static const double a51=19372.0/6561, a52=-25360.0/2187, a53=64448.0/6561, a54=-212.0/729;
    static const double a61=9017.0/3168, a62=-355.0/33, a63=46732.0/5247, a64=49.0/176, a65=-5103.0/18656;
    static const double a71=35.0/384, a73=500.0/1113, a74=125.0/192, a75=-2187.0/6784, a76=11.0/84;
    // 5阶解b = a7*（FSAL），4阶误差系数
    static const double e1=71.0/57600, e3=-71.0/16695, e4=71.0/1920, e5=-17253.0/339200, e6=22.0/525, e7=-1.0/40;

    DynArr d = {0};
    double t = t0, y = y0, h = h_init;
    dyn_push(&d, t, y);
    while (t < t_end) {
        if (t + h > t_end) h = t_end - t;
        double k1 = f(t, y);
        double k2 = f(t+c2*h, y+h*a21*k1);
        double k3 = f(t+c3*h, y+h*(a31*k1+a32*k2));
        double k4 = f(t+c4*h, y+h*(a41*k1+a42*k2+a43*k3));
        double k5 = f(t+c5*h, y+h*(a51*k1+a52*k2+a53*k3+a54*k4));
        double k6 = f(t+c6*h, y+h*(a61*k1+a62*k2+a63*k3+a64*k4+a65*k5));
        double y_new = y + h*(a71*k1 + a73*k3 + a74*k4 + a75*k5 + a76*k6);
        double k7 = f(t+h, y_new);
        double err = fabs(h*(e1*k1 + e3*k3 + e4*k4 + e5*k5 + e6*k6 + e7*k7));
        if (err < tol || h < 1e-12) {
            t += h; y = y_new;
            dyn_push(&d, t, y);
        }
        double factor = 0.9 * pow(tol/(err+1e-16), 0.2);
        if (factor > 5) factor = 5;
        if (factor < 0.1) factor = 0.1;
        h *= factor;
    }
    ODEResult *r = ode_result_create(d.size);
    memcpy(r->t, d.t, d.size*sizeof(double));
    memcpy(r->y, d.y, d.size*sizeof(double));
    free(d.t); free(d.y);
    return r;
}
