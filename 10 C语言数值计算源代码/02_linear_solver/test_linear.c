/**
 * @file test_linear.c
 * @brief 线性方程组求解验证：对比4种算法在不同规模/测试数据上的精度与效率
 */
#include "linear_solver.h"

static void test_3x3() {
    printf("\n========== 【测试1】3x3方程组（经典教材用例） ==========\n");
    printf("求解: \n");
    printf("  2x1 +  x2 -  x3 =  1\n");
    printf("  4x1 + 3x2 -  x3 =  5\n");
    printf("  3x1 + 2x2 - 3x3 = -1\n");
    double Ad[3][3] = {{2,1,-1},{4,3,-1},{3,2,-3}};
    double bd[3] = {1,5,-1};
    Matrix *A = mat_create(3,3); Vector *b = vec_create(3);
    for(int i=0;i<3;i++) for(int j=0;j<3;j++) A->data[i][j]=Ad[i][j];
    for(int i=0;i<3;i++) b->data[i]=bd[i];

    // 精确解 x=(1,1,2)
    Vector *xe = vec_create(3);
    xe->data[0]=1; xe->data[1]=1; xe->data[2]=2;
    vec_print("精确解 x*", xe, 6);

    Vector *xg = vec_create(3);
    gauss_solve(A, b, xg);
    vec_print("高斯消元解", xg, 6);
    printf("误差 L2=%.2e : %s\n", vec_error_l2(xg, xe), verify_vector(xg, xe, 1e-8)?"✓ PASS":"✗ FAIL");

    Vector *xj = vec_create(3); for(int i=0;i<3;i++)xj->data[i]=0;
    int itj = jacobi_solve(A, b, xj, 1e-10, 1000);
    vec_print("雅可比迭代解", xj, 6);
    printf("迭代%d次, 误差L2=%.2e : %s\n", itj, vec_error_l2(xj,xe), verify_vector(xj,xe,1e-6)?"✓ PASS":"✗ FAIL");

    Vector *xgs = vec_create(3); for(int i=0;i<3;i++)xgs->data[i]=0;
    int itgs = gauss_seidel_solve(A, b, xgs, 1e-10, 1000);
    vec_print("高斯-赛德尔解", xgs, 6);
    printf("迭代%d次, 误差L2=%.2e : %s\n", itgs, vec_error_l2(xgs,xe), verify_vector(xgs,xe,1e-6)?"✓ PASS":"✗ FAIL");

    mat_free(A); vec_free(b); vec_free(xe); vec_free(xg); vec_free(xj); vec_free(xgs);
}

static void test_spd_system() {
    printf("\n========== 【测试2】对称正定稀疏矩阵（泊松方程5点差分 N=50，2500阶） ==========\n");
    int N = 50, n = N*N;
    printf("生成对称正定 %d x %d 带状矩阵...\n", n, n);
    Matrix *A = mat_create(n, n);
    Vector *b = vec_create(n);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int k = i*N + j;
            A->data[k][k] = 4.0;
            if (i > 0)   A->data[k][k-N] = -1.0;
            if (i < N-1) A->data[k][k+N] = -1.0;
            if (j > 0)   A->data[k][k-1] = -1.0;
            if (j < N-1) A->data[k][k+1] = -1.0;
            b->data[k] = 1.0; // 右边全1
        }
    }
    // 用高斯消元作基准
    Vector *xg = vec_create(n);
    double t0 = get_time_sec();
    gauss_solve(A, b, xg);
    double tg = get_time_sec() - t0;
    printf("高斯消元耗时 %.4f 秒\n", tg);

    Vector *xcg = vec_create(n); for(int i=0;i<n;i++)xcg->data[i]=0;
    t0 = get_time_sec();
    int itcg = cg_solve(A, b, xcg, 1e-8, n);
    double tcg = get_time_sec() - t0;
    vec_print("CG解（前10维）", xcg, 8);
    printf("CG 迭代%d次, 耗时%.4f秒, 误差L2=%.2e : %s\n",
           itcg, tcg, vec_error_l2(xcg, xg), verify_vector(xcg, xg, 1e-4)?"✓ PASS":"✗ FAIL");

    mat_free(A); vec_free(b); vec_free(xg); vec_free(xcg);
}

static void test_from_file() {
    printf("\n========== 【测试3】从测试数据读取 Hilbert 矩阵（病态） ==========\n");
    Matrix *A = mat_read("test_data/hilbert10.txt");
    Vector *b = vec_read("test_data/hilbert10_b.txt");
    if (!A || !b) { printf("测试数据不存在，跳过\n"); if(A)mat_free(A); if(b)vec_free(b); return; }
    Vector *xe = vec_read("test_data/hilbert10_x.txt");
    Vector *x = vec_create(A->rows);
    gauss_solve(A, b, x);
    printf("Hilbert 10阶矩阵条件数极大，验证解精度：\n");
    printf("误差 L2=%.2e\n", vec_error_l2(x, xe));
    vec_print("计算解前5维", x, 6);
    vec_print("精确解前5维", xe, 6);
    mat_free(A); vec_free(b); vec_free(xe); vec_free(x);
}

int main() {
    test_3x3();
    test_spd_system();
    test_from_file();
    printf("\n========== 线性方程组求解全部验证完成 ==========\n");
    return 0;
}
