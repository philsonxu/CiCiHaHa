/**
 * @file test_interpolation.c
 * @brief 插值验证：Runge现象演示 + 从测试数据读取真实节点
 */
#include "interpolation.h"
#include <math.h>

static double runge(double x) { return 1.0/(1.0 + 25.0*x*x); } // Runge函数

static void test_runge() {
    printf("\n========== 【测试1】Runge现象 f(x)=1/(1+25x^2), x∈[-1,1] ==========\n");
    int n = 11; // 11节点等距
    double xs[11], ys[11];
    for (int i = 0; i < n; i++) { xs[i] = -1.0 + 2.0*i/(n-1); ys[i] = runge(xs[i]); }
    PolyInterp *p = poly_interp_create(xs, ys, n);
    Spline *sp = spline_create(xs, ys, n);

    printf("在5个均匀点上对比拉格朗日插值 vs 三次样条 vs 真值:\n");
    printf("%8s  %12s  %12s  %12s  %12s\n", "x", "真值", "拉格朗日", "三次样条", "拉格朗日误差");
    for (int k = 0; k <= 8; k++) {
        double x = -0.8 + 1.6*k/8;
        double yt = runge(x);
        double yl = lagrange_eval(p, x);
        double ys2 = spline_eval(sp, x);
        printf("%8.3f  %12.8f  %12.8f  %12.8f  %12.2e\n", x, yt, yl, ys2, fabs(yl-yt));
    }
    printf("\n结论：等距高次拉格朗日在区间端点出现Runge振荡，三次样条稳定。\n");
    poly_interp_free(p);
    spline_free(sp);
}

static void test_chebyshev() {
    printf("\n========== 【测试2】切比雪夫节点抑制Runge现象（11点） ==========\n");
    int n = 11;
    double xs[11], ys[11];
    for (int i = 0; i < n; i++) {
        xs[i] = cos(M_PI*(2*i+1)/(2*n)); // 切比雪夫节点
        ys[i] = runge(xs[i]);
    }
    PolyInterp *p = poly_interp_create(xs, ys, n);
    double max_err = 0;
    for (int k = 0; k <= 200; k++) {
        double x = -1 + 2.0*k/200;
        double err = fabs(lagrange_eval(p, x) - runge(x));
        if (err > max_err) max_err = err;
    }
    printf("切比雪夫节点拉格朗日插值最大误差: %.2e\n", max_err);
    poly_interp_free(p);
}

static void test_from_file() {
    printf("\n========== 【测试3】从测试数据读取采样点（正弦函数5等距点） ==========\n");
    Vector *xdata = vec_read("test_data/sine_x.txt");
    Vector *ydata = vec_read("test_data/sine_y.txt");
    if (!xdata || !ydata) { printf("测试数据不存在\n"); return; }
    PolyInterp *p = poly_interp_create(xdata->data, ydata->data, xdata->size);
    Spline *sp = spline_create(xdata->data, ydata->data, xdata->size);
    printf("在 x=pi/4, pi/2, 3pi/4 处验证 (精确值sin(x)):\n");
    double pts[] = {M_PI/4, M_PI/2, 3*M_PI/4};
    for (int i = 0; i < 3; i++) {
        double x = pts[i], yt = sin(x);
        double yl = lagrange_eval(p, x);
        double yn = newton_eval(p, x);
        double ys = spline_eval(sp, x);
        printf("  x=%.4f (sin=%.6f): 拉格朗日=%.6f(%.1e) 牛顿=%.6f(%.1e) 样条=%.6f(%.1e)\n",
               x, yt, yl, fabs(yl-yt), yn, fabs(yn-yt), ys, fabs(ys-yt));
    }
    poly_interp_free(p); spline_free(sp);
    vec_free(xdata); vec_free(ydata);
}

int main() {
    test_runge();
    test_chebyshev();
    test_from_file();
    printf("\n========== 插值全部验证完成 ==========\n");
    return 0;
}
