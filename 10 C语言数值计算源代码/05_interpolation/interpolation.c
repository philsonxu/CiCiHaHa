/**
 * @file interpolation.c
 * @brief 插值算法实现（牛顿差商在创建时预计算）
 */
#include "interpolation.h"
#include <stdlib.h>
#include <string.h>

PolyInterp* poly_interp_create(const double *xs, const double *ys, int n) {
    PolyInterp *p = (PolyInterp*)malloc(sizeof(PolyInterp));
    p->n = n;
    p->x = (double*)malloc(n*sizeof(double));
    p->y = (double*)malloc(n*sizeof(double));
    memcpy(p->x, xs, n*sizeof(double));
    memcpy(p->y, ys, n*sizeof(double));
    return p;
}

void poly_interp_free(PolyInterp *p) {
    if (!p) return;
    free(p->x); free(p->y); free(p);
}

double lagrange_eval(const PolyInterp *p, double x) {
    double res = 0;
    for (int i = 0; i < p->n; i++) {
        double li = 1.0;
        for (int j = 0; j < p->n; j++) {
            if (j != i) li *= (x - p->x[j]) / (p->x[i] - p->x[j]);
        }
        res += p->y[i] * li;
    }
    return res;
}

/* ---------------- 牛顿差商插值 ---------------- */
typedef struct {
    PolyInterp base;
    double *coeff; // 差商系数
} NewtonInterp;

static double* newton_diff(const double *x, const double *y, int n) {
    double *dd = (double*)malloc(n*sizeof(double));
    double *col = (double*)malloc(n*sizeof(double));
    memcpy(col, y, n*sizeof(double));
    dd[0] = col[0];
    for (int j = 1; j < n; j++) {
        for (int i = n-1; i >= j; i--) {
            col[i] = (col[i] - col[i-1]) / (x[i] - x[i-j]);
        }
        dd[j] = col[j];
    }
    free(col);
    return dd;
}

/** 封装：用牛顿法时需用此函数创建（内部预计算差商） */
static PolyInterp* newton_create(const double *xs, const double *ys, int n) {
    NewtonInterp *np = (NewtonInterp*)malloc(sizeof(NewtonInterp));
    np->base.n = n;
    np->base.x = (double*)malloc(n*sizeof(double));
    np->base.y = (double*)malloc(n*sizeof(double));
    memcpy(np->base.x, xs, n*sizeof(double));
    memcpy(np->base.y, ys, n*sizeof(double));
    np->coeff = newton_diff(xs, ys, n);
    return (PolyInterp*)np;
}

double newton_eval(const PolyInterp *p, double x) {
    // 简化：直接重新计算（教学版，避免指针类型转换复杂度）
    int n = p->n;
    double *dd = newton_diff(p->x, p->y, n);
    double res = dd[n-1];
    for (int i = n-2; i >= 0; i--) res = res*(x - p->x[i]) + dd[i];
    free(dd);
    return res;
}

/* ---------------- 三次样条（自然边界 S''(x0)=S''(xn)=0） ---------------- */
Spline* spline_create(const double *xs, const double *ys, int n) {
    Spline *s = (Spline*)malloc(sizeof(Spline));
    s->n = n;
    s->x = (double*)malloc(n*sizeof(double));
    s->y = (double*)malloc(n*sizeof(double));
    s->a = (double*)calloc(n, sizeof(double));
    s->b = (double*)calloc(n, sizeof(double));
    s->c = (double*)calloc(n, sizeof(double));
    s->d = (double*)calloc(n, sizeof(double));
    memcpy(s->x, xs, n*sizeof(double));
    memcpy(s->y, ys, n*sizeof(double));
    if (n < 3) { // 两点退化为线性
        s->b[0] = (ys[1]-ys[0])/(xs[1]-xs[0]);
        return s;
    }
    int m = n - 1;
    double *h = (double*)malloc(m*sizeof(double));
    for (int i = 0; i < m; i++) h[i] = xs[i+1] - xs[i];
    // 三对角方程组求c（自然边界 c0=cn=0）
    double *alpha = (double*)calloc(n, sizeof(double));
    double *l     = (double*)calloc(n, sizeof(double));
    double *mu    = (double*)calloc(n, sizeof(double));
    double *z     = (double*)calloc(n, sizeof(double));
    for (int i = 1; i < m; i++)
        alpha[i] = 3.0/h[i]*(ys[i+1]-ys[i]) - 3.0/h[i-1]*(ys[i]-ys[i-1]);
    l[0] = 1;
    for (int i = 1; i < m; i++) {
        l[i] = 2*(xs[i+1]-xs[i-1]) - h[i-1]*mu[i-1];
        mu[i] = h[i]/l[i];
        z[i] = (alpha[i] - h[i-1]*z[i-1])/l[i];
    }
    l[m] = 1;
    for (int j = m-1; j >= 0; j--) {
        s->c[j] = z[j] - mu[j]*s->c[j+1];
        s->b[j] = (ys[j+1]-ys[j])/h[j] - h[j]*(s->c[j+1] + 2*s->c[j])/3.0;
        s->d[j] = (s->c[j+1] - s->c[j])/(3*h[j]);
        s->a[j] = ys[j];
    }
    free(h); free(alpha); free(l); free(mu); free(z);
    return s;
}

void spline_free(Spline *s) {
    if (!s) return;
    free(s->x); free(s->y); free(s->a); free(s->b); free(s->c); free(s->d);
    free(s);
}

double spline_eval(const Spline *s, double x) {
    int n = s->n;
    int i = 0, j = n-2;
    while (i < j) { // 二分查找所属区间
        int mid = (i+j)/2;
        if (x > s->x[mid+1]) i = mid+1; else j = mid;
    }
    double dx = x - s->x[i];
    return s->a[i] + s->b[i]*dx + s->c[i]*dx*dx + s->d[i]*dx*dx*dx;
}
