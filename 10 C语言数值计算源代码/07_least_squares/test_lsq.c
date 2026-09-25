/**
 * @file test_lsq.c
 * @brief 最小二乘拟合验证：内置线性/二次/三次数据 + 读取外部带噪声数据
 */
#include "least_squares.h"

static void test_exact() {
    printf("\n========== 【测试1】精确二次函数 y=1+2x+3x^2 无噪声拟合 ==========\n");
    int n = 20;
    double x[20], y[20];
    for (int i = 0; i < n; i++) {
        x[i] = -1.0 + 2.0*i/(n-1);
        y[i] = 1 + 2*x[i] + 3*x[i]*x[i];
    }
    double c[4]; // m=2, 3个系数
    poly_fit(x, y, n, 2, c);
    printf("拟合系数 c0,c1,c2 = %.6f, %.6f, %.6f (理论值 1,2,3)\n", c[0], c[1], c[2]);
    printf("R^2 = %.12f\n", r_squared(x, y, n, c, 2));
    print_result("c0", c[0], 1.0, 1e-8);
    print_result("c1", c[1], 2.0, 1e-8);
    print_result("c2", c[2], 3.0, 1e-8);
}

static void test_noisy() {
    printf("\n========== 【测试2】带噪声线性数据 y=2x+1+N(0,0.1) ==========\n");
    int n = 50;
    srand(12345);
    double x[50], y[50];
    for (int i = 0; i < n; i++) {
        x[i] = (double)i / (n-1) * 5.0;
        y[i] = 1.0 + 2.0*x[i] + ((double)rand()/RAND_MAX - 0.5)*0.2;
    }
    double c[2];
    poly_fit(x, y, n, 1, c);
    printf("线性拟合 y = %.4f + %.4f x  (理论: 1 + 2x)\n", c[0], c[1]);
    printf("R^2 = %.6f\n", r_squared(x, y, n, c, 1));

    double c3[4];
    poly_fit(x, y, n, 3, c3);
    printf("3次拟合 R^2 = %.6f\n", r_squared(x, y, n, c3, 3));
}

static void test_from_file() {
    printf("\n========== 【测试3】从文件读取带噪声二次数据 ==========\n");
    Vector *xv = vec_read("test_data/quad_noisy_x.txt");
    Vector *yv = vec_read("test_data/quad_noisy_y.txt");
    if (!xv || !yv) { printf("测试数据不存在\n"); if(xv)vec_free(xv); if(yv)vec_free(yv); return; }
    int n = xv->size;
    double c1[2], c2[3], c4[5];
    poly_fit(xv->data, yv->data, n, 1, c1);
    poly_fit(xv->data, yv->data, n, 2, c2);
    poly_fit(xv->data, yv->data, n, 4, c4);
    printf("数据点数: %d, 底层生成 y=0.5 - x + 2x^2 + N(0,0.3)\n", n);
    printf("1次拟合 R^2=%.6f  y=%.4f%+.4fx\n", r_squared(xv->data,yv->data,n,c1,1), c1[0], c1[1]);
    printf("2次拟合 R^2=%.6f  y=%.4f%+.4fx%+.4fx^2  (理论 0.5,-1,2)\n",
           r_squared(xv->data,yv->data,n,c2,2), c2[0], c2[1], c2[2]);
    printf("4次拟合 R^2=%.6f\n", r_squared(xv->data,yv->data,n,c4,4));
    vec_free(xv); vec_free(yv);
}

int main() {
    test_exact();
    test_noisy();
    test_from_file();
    printf("\n========== 最小二乘拟合全部验证完成 ==========\n");
    return 0;
}
