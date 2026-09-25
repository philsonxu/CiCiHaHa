/**
 * @file root_finding.c
 * @brief 非线性方程求根实现
 */
#include "root_finding.h"

double bisection(Func1D f, double a, double b, double tol, int max_iter, int *iter) {
    double fa = f(a), fb = f(b), c, fc;
    *iter = 0;
    if (fa * fb > 0) { fprintf(stderr, "二分法错误：端点函数值同号\n"); return NAN; }
    for (*iter = 0; *iter < max_iter; (*iter)++) {
        c = (a + b) / 2.0;
        fc = f(c);
        if (fabs(fc) < tol || (b - a) / 2.0 < tol) return c;
        if (fa * fc < 0) { b = c; fb = fc; }
        else { a = c; fa = fc; }
    }
    return c;
}

double newton_method(Func1D f, Func1DD df, double x0, double tol, int max_iter, int *iter) {
    double x = x0;
    for (*iter = 0; *iter < max_iter; (*iter)++) {
        double fx = f(x);
        if (fabs(fx) < tol) return x;
        double dfx = df(x);
        if (fabs(dfx) < EPS) { fprintf(stderr, "牛顿法警告：导数接近0\n"); return x; }
        double x_new = x - fx / dfx;
        if (fabs(x_new - x) < tol) return x_new;
        x = x_new;
    }
    return x;
}

double secant_method(Func1D f, double x0, double x1, double tol, int max_iter, int *iter) {
    double f0 = f(x0), f1 = f(x1), x2;
    for (*iter = 0; *iter < max_iter; (*iter)++) {
        if (fabs(f1) < tol) return x1;
        if (fabs(f1 - f0) < EPS) { fprintf(stderr, "割线法警告：除零\n"); return x1; }
        x2 = x1 - f1 * (x1 - x0) / (f1 - f0);
        double f2 = f(x2);
        x0 = x1; f0 = f1;
        x1 = x2; f1 = f2;
        if (fabs(x1 - x0) < tol) return x1;
    }
    return x1;
}

double fixed_point(Func1D g, double x0, double tol, int max_iter, int *iter) {
    double x = x0;
    for (*iter = 0; *iter < max_iter; (*iter)++) {
        double x_new = g(x);
        if (fabs(x_new - x) < tol) return x_new;
        x = x_new;
    }
    return x;
}
