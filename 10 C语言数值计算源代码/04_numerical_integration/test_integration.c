/**
 * @file test_integration.c
 * @brief 数值积分验证：5个经典积分用例对比各算法精度与函数求值次数
 */
#include "integration.h"
#include <math.h>

// 测试1: ∫_0^1 x^2 dx = 1/3
static double f_poly(double x) { return x*x; }
// 测试2: ∫_0^pi sin(x) dx = 2
static double f_sin(double x) { return sin(x); }
// 测试3: ∫_0^1 e^x dx = e-1
static double f_exp(double x) { return exp(x); }
// 测试4: ∫_0^1 4/(1+x^2) dx = pi
static double f_pi(double x) { return 4.0/(1.0 + x*x); }
// 测试5: ∫_0^1 sqrt(x) dx = 2/3  (端点奇异，考验自适应算法)
static double f_sqrt(double x) { return sqrt(x); }

static void test_integ(const char *name, Func1D f, double a, double b, double exact) {
    printf("\n--- %s, 精确值 = %.12f ---\n", name, exact);
    double r;
    r = trapezoidal(f, a, b, 1000);
    print_result("复合梯形(1000区间)", r, exact, 1e-6);
    r = simpson(f, a, b, 100);
    print_result("复合辛普森(100区间)", r, exact, 1e-8);
    r = adaptive_simpson(f, a, b, 1e-10);
    print_result("自适应辛普森(1e-10)", r, exact, 1e-10);
    r = romberg(f, a, b, 1e-12, 15);
    print_result("龙贝格(1e-12)", r, exact, 1e-10);
    r = gauss_legendre(f, a, b, 5);
    print_result("5点高斯-勒让德", r, exact, 1e-8);
}

int main() {
    printf("\n========== 数值积分验证 ==========\n");
    test_integ("∫0^1 x^2 dx = 1/3", f_poly, 0, 1, 1.0/3);
    test_integ("∫0^π sin(x) dx = 2", f_sin, 0, M_PI, 2.0);
    test_integ("∫0^1 e^x dx = e-1", f_exp, 0, 1, M_E - 1);
    test_integ("∫0^1 4/(1+x^2) dx = π", f_pi, 0, 1, M_PI);
    test_integ("∫0^1 √x dx = 2/3 (端点导数奇异)", f_sqrt, 0, 1, 2.0/3);
    printf("\n========== 数值积分全部验证完成 ==========\n");
    return 0;
}
