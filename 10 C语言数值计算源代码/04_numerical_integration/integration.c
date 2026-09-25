/**
 * @file integration.c
 * @brief 一维数值积分实现
 */
#include "integration.h"

double trapezoidal(Func1D f, double a, double b, int n) {
    double h = (b - a) / n;
    double s = 0.5 * (f(a) + f(b));
    for (int i = 1; i < n; i++) s += f(a + i*h);
    return s * h;
}

double simpson(Func1D f, double a, double b, int n) {
    if (n % 2 != 0) n++;
    double h = (b - a) / n;
    double s = f(a) + f(b);
    for (int i = 1; i < n; i += 2) s += 4 * f(a + i*h);
    for (int i = 2; i < n; i += 2) s += 2 * f(a + i*h);
    return s * h / 3.0;
}

/** @brief 递归核心：辛普森区间自适应 */
static double adaptive_simpson_rec(Func1D f, double a, double b, double eps,
                                   double S, double fa, double fb, double fc, int depth) {
    double c = (a + b) / 2.0, d = (a + c) / 2.0, e = (c + b) / 2.0;
    double fd = f(d), fe = f(e);
    double Sleft  = (c - a)/6.0 * (fa + 4*fd + fc);
    double Sright = (b - c)/6.0 * (fc + 4*fe + fb);
    double S2 = Sleft + Sright;
    if (depth > 30 || fabs(S2 - S) <= 15*eps)
        return S2 + (S2 - S)/15.0;
    return adaptive_simpson_rec(f, a, c, eps/2.0, Sleft,  fa, fc, fd, depth+1)
         + adaptive_simpson_rec(f, c, b, eps/2.0, Sright, fc, fb, fe, depth+1);
}

double adaptive_simpson(Func1D f, double a, double b, double tol) {
    double c = (a + b) / 2.0;
    double fa = f(a), fb = f(b), fc = f(c);
    double S = (b - a)/6.0 * (fa + 4*fc + fb);
    return adaptive_simpson_rec(f, a, b, tol, S, fa, fb, fc, 0);
}

double romberg(Func1D f, double a, double b, double tol, int max_level) {
    double R[20][20] = {0};
    int n = 1;
    double h = b - a;
    R[0][0] = h/2.0 * (f(a) + f(b));
    for (int i = 1; i < max_level; i++) {
        h /= 2.0;
        double sum = 0;
        for (int k = 1; k <= n; k++) sum += f(a + (2*k-1)*h);
        R[i][0] = 0.5*R[i-1][0] + h*sum;
        n *= 2;
        for (int j = 1; j <= i; j++) {
            R[i][j] = R[i][j-1] + (R[i][j-1] - R[i-1][j-1]) / ((1 << (2*j)) - 1);
        }
        if (fabs(R[i][i] - R[i-1][i-1]) < tol) return R[i][i];
    }
    return R[max_level-1][max_level-1];
}

double gauss_legendre(Func1D f, double a, double b, int n) {
    // 内置n=2/3/4/5/8/16节点与权重
    static const double *x[17], *w[17];
    static double x2[]={-0.5773502691896257, 0.5773502691896257};
    static double w2[]={1.0, 1.0};
    static double x3[]={-0.7745966692414834,0,0.7745966692414834};
    static double w3[]={5.0/9, 8.0/9, 5.0/9};
    static double x4[]={-0.8611363115940526,-0.3399810435848563,0.3399810435848563,0.8611363115940526};
    static double w4[]={0.3478548451374538,0.6521451548625461,0.6521451548625461,0.3478548451374538};
    static double x5[]={-0.9061798459386640,-0.5384693101056831,0,0.5384693101056831,0.9061798459386640};
    static double w5[]={0.2369268850561891,0.4786286704993665,0.5688888888888889,0.4786286704993665,0.2369268850561891};
    x[2]=x2;w[2]=w2;x[3]=x3;w[3]=w3;x[4]=x4;w[4]=w4;x[5]=x5;w[5]=w5;
    if (n!=2 && n!=3 && n!=4 && n!=5) {
        fprintf(stderr, "高斯求积目前支持n=2/3/4/5\n"); return NAN;
    }
    double half = (b - a) / 2.0, mid = (a + b) / 2.0;
    double s = 0;
    for (int i = 0; i < n; i++) s += w[n][i] * f(mid + half * x[n][i]);
    return s * half;
}
