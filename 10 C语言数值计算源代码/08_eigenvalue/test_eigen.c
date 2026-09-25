/**
 * @file test_eigen.c
 * @brief 特征值验证：2x2/3x3小矩阵 + 对称矩阵全特征值Jacobi
 */
#include "eigenvalue.h"
#include <math.h>

static int cmp_double(const void *a, const void *b){return *(double*)a<*(double*)b?-1:1;}

static void test_power() {
    printf("\n========== 【测试1】幂法/反幂法 3x3矩阵 ==========\n");
    // 矩阵 [[2,1,0],[1,3,1],[0,1,2]]
    // 特征多项式=(2-λ)(4-λ)(1-λ)，特征值为 1,2,4
    double Ad[3][3]={{2,1,0},{1,3,1},{0,1,2}};
    Matrix *A = mat_create(3,3);
    for(int i=0;i<3;i++)for(int j=0;j<3;j++)A->data[i][j]=Ad[i][j];
    mat_print("A", A, 4);
    printf("理论特征值: 1, 2, 4\n");

    int iter;
    double vec[3];
    double lam_max = power_iteration(A, vec, 1e-10, 1000, &iter);
    printf("\n幂法（最大模）:  λ=%.10f  迭代%d次\n", lam_max, iter);
    printf("特征向量: [%.6f, %.6f, %.6f]  （应为[1,√2,1]比例）\n", vec[0],vec[1],vec[2]);
    print_result("最大特征值", lam_max, 4.0, 1e-8);

    double lam_min = inverse_iteration(A, vec, 1e-10, 1000, &iter);
    printf("\n反幂法（最小模）: λ=%.10f  迭代%d次\n", lam_min, iter);
    print_result("最小特征值", lam_min, 1.0, 1e-6);
    mat_free(A);
}

static void test_jacobi() {
    printf("\n========== 【测试2】Jacobi法求实对称矩阵全部特征值 ==========\n");
    // 4x4对称矩阵
    double Ad[4][4]={{4,1,2,1},{1,3,0,1},{2,0,2,1},{1,1,1,1}};
    Matrix *A=mat_create(4,4);
    for(int i=0;i<4;i++)for(int j=0;j<4;j++)A->data[i][j]=Ad[i][j];
    mat_print("对称矩阵A", A, 4);

    double evals[4];
    Matrix *evecs = mat_create(4,4);
    jacobi_eigen(A, evals, evecs, 1e-10);

    qsort(evals, 4, sizeof(double), cmp_double);
    printf("Jacobi法求得的特征值（升序）:\n");
    for(int i=0;i<4;i++) printf("  λ[%d] = %.10f\n", i, evals[i]);
    mat_print("特征向量矩阵（列向量）", evecs, 6);

    // 验证 A*v_i ≈ λ_i*v_i （用最小特征值验证）
    // 找最小特征值对应列
    int idx=0; double minv=evals[0];
    // 简单检查A*v_i ≈ λ*v_i
    double max_err=0;
    // 重新跑一次获取对应列顺序
    jacobi_eigen(A, evals, evecs, 1e-10); // 列对应原顺序
    printf("\n验证 A*v_i = λ_i*v_i (逐列):\n");
    for(int j=0;j<4;j++){
        double err=0;
        for(int i=0;i<4;i++){
            double s=0;for(int k=0;k<4;k++)s+=A->data[i][k]*evecs->data[k][j];
            err+=fabs(s - evals[j]*evecs->data[i][j]);
        }
        if(err>max_err)max_err=err;
        printf("  列%d λ=%.6f 残差和=%.2e\n", j, evals[j], err);
    }
    printf("最大残差和: %.2e : %s\n", max_err, max_err<1e-6?"✓ PASS":"✗ FAIL");

    mat_free(A); mat_free(evecs);
}

int main() {
    test_power();
    test_jacobi();
    printf("\n========== 特征值计算全部验证完成 ==========\n");
    return 0;
}
