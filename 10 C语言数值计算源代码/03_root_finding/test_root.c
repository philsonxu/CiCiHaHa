/**
 * @file test_root.c
 * @brief 非线性方程求根验证：覆盖4种典型方程对比各方法迭代次数与精度
 */
#include "root_finding.h"
#include <math.h>

static double f1(double x) { return x*x - 2; }           // x^2=2, 根=sqrt(2)≈1.41421356
static double f1d(double x){ return 2*x; }
static double f2(double x) { return cos(x) - x; }        // cos(x)=x, 根≈0.739085 (Dottie数)
static double f2d(double x){ return -sin(x) - 1; }
static double f3(double x) { return x*x*x - x - 2; }     // x^3-x-2=0, 根≈1.521380
static double f3d(double x){ return 3*x*x - 1; }
static double g_sqrt2(double x) { return (x + 2.0/x) / 2.0; } // 求sqrt(2)的不动点迭代

static void test_case(const char *name, Func1D f, Func1DD df,
                      double a, double b, double x0, double x1,
                      double root_true) {
    printf("\n--- %s (理论根 = %.10f) ---\n", name, root_true);
    int iter;
    double r;
    r = bisection(f, a, b, 1e-12, 200, &iter);
    print_result("二分法", r, root_true, 1e-10);
    printf("           迭代次数: %d\n", iter);

    r = newton_method(f, df, x0, 1e-12, 200, &iter);
    print_result("牛顿法", r, root_true, 1e-10);
    printf("           迭代次数: %d\n", iter);

    r = secant_method(f, x0, x1, 1e-12, 200, &iter);
    print_result("割线法", r, root_true, 1e-10);
    printf("           迭代次数: %d\n", iter);
}

int main() {
    printf("\n========== 非线性方程求根验证 ==========\n");

    test_case("方程1: x^2 - 2 = 0",
              f1, f1d,
              1.0, 2.0,  // 二分区间
              1.0, 2.0,  // 牛顿初值/割线初值对
              sqrt(2.0));

    test_case("方程2: cos(x) - x = 0 (Dottie数)",
              f2, f2d,
              0.0, 1.5,
              0.5, 1.0,
              0.7390851332);

    test_case("方程3: x^3 - x - 2 = 0",
              f3, f3d,
              1.0, 2.0,
              1.0, 2.0,
              1.5213797068);

    // 不动点迭代：求 sqrt(2)，g(x) = (x+2/x)/2
    printf("\n--- 不动点迭代求 g(x)=(x+2/x)/2 -> sqrt(2) ---\n");
    int iter;
    double r = fixed_point(g_sqrt2, 1.0, 1e-12, 100, &iter);
    print_result("不动点迭代", r, sqrt(2.0), 1e-12);
    printf("           迭代次数: %d (牛顿法形态，二次收敛)\n", iter);

    printf("\n========== 非线性方程求根全部验证完成 ==========\n");
    return 0;
}
