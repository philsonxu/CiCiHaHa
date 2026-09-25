/**
 * @file test_matrix.c
 * @brief 矩阵运算验证程序：内置测试用例 + 从test_data读取外部测试数据
 * 验证项：加法/减法/乘法/转置/行列式/LU分解/求逆（A * A^{-1} = I）
 */
#include "matrix_ops.h"

static void test_basic_ops() {
    printf("\n========== 【测试1】矩阵基础运算（3x3内置用例） ==========\n");
    // A = [[1,2,3],[4,5,6],[7,8,10]]  B = [[1,0,0],[0,1,0],[0,0,1]]
    double Ad[3][3] = {{1,2,3},{4,5,6},{7,8,10}};
    double Bd[3][3] = {{1,0,0},{0,1,0},{0,0,1}};
    Matrix *A = mat_create(3,3), *B = mat_create(3,3);
    for (int i=0;i<3;i++) for(int j=0;j<3;j++){A->data[i][j]=Ad[i][j]; B->data[i][j]=Bd[i][j];}
    mat_print("A", A, 4);
    mat_print("B (单位矩阵)", B, 1);

    Matrix *C = mat_add(A, B);  mat_print("A+B", C, 4);
    Matrix *D = mat_sub(A, B);  mat_print("A-B", D, 4);
    Matrix *E = mat_mul(A, B);  mat_print("A*B", E, 4);
    Matrix *T = mat_transpose(A); mat_print("A^T", T, 4);

    // 验证：A*B应等于A
    printf("验证 A*B == A : %s\n", verify_matrix(E, A, 1e-10) ? "✓ PASS" : "✗ FAIL");
    double detA = mat_det(A);
    print_result("det(A)", detA, -3.0, EPS);

    // LU分解验证 PA = LU
    Matrix *L=mat_create(3,3), *U=mat_create(3,3), *P=mat_create(3,3);
    mat_lu_decomp(A, L, U, P);
    mat_print("L (单位下三角)", L, 4);
    mat_print("U (上三角)", U, 4);
    mat_print("P (置换矩阵)", P, 0);
    Matrix *PA = mat_mul(P, A);
    Matrix *LU = mat_mul(L, U);
    printf("验证 PA == LU (误差F范数: %.2e): %s\n",
           mat_error_fro(PA, LU), verify_matrix(PA, LU, 1e-8) ? "✓ PASS" : "✗ FAIL");

    // 求逆验证 A*A^{-1} = I
    Matrix *invA = mat_inverse(A);
    mat_print("A^{-1}", invA, 6);
    Matrix *I_calc = mat_mul(A, invA);
    mat_print("A * A^{-1}", I_calc, 6);
    printf("验证 A*A^{-1} == I : %s\n", verify_matrix(I_calc, B, 1e-8) ? "✓ PASS" : "✗ FAIL");

    mat_free(A); mat_free(B); mat_free(C); mat_free(D); mat_free(E); mat_free(T);
    mat_free(L); mat_free(U); mat_free(P); mat_free(PA); mat_free(LU); mat_free(invA); mat_free(I_calc);
}

static void test_from_file() {
    printf("\n========== 【测试2】从测试数据文件读取 4x4 矩阵 ==========\n");
    Matrix *A = mat_read("test_data/mat4x4.txt");
    if (!A) { printf("测试数据文件不存在，跳过\n"); return; }
    mat_print("读取的4x4矩阵A", A, 4);
    double detA = mat_det(A);
    printf("det(A) = %.8f\n", detA);

    Matrix *invA = mat_inverse(A);
    Matrix *I = mat_mul(A, invA);
    Matrix *Ie = mat_create(4,4); for(int i=0;i<4;i++) Ie->data[i][i]=1.0;
    printf("验证 4x4 A*A^{-1}==I (误差F范数: %.2e): %s\n",
           mat_error_fro(I, Ie), verify_matrix(I, Ie, 1e-8) ? "✓ PASS" : "✗ FAIL");

    mat_free(A); mat_free(invA); mat_free(I); mat_free(Ie);
}

static void test_large_matrix() {
    printf("\n========== 【测试3】大矩阵乘法性能 200x200 ==========\n");
    int n = 200;
    Matrix *A = mat_create(n,n), *B = mat_create(n,n);
    srand(42);
    for(int i=0;i<n;i++) for(int j=0;j<n;j++){A->data[i][j]=(double)rand()/RAND_MAX; B->data[i][j]=(double)rand()/RAND_MAX;}
    double t0 = get_time_sec();
    Matrix *C = mat_mul(A, B);
    double t1 = get_time_sec();
    printf("200x200 矩阵乘法耗时: %.4f 秒\n", t1 - t0);
    mat_free(A); mat_free(B); mat_free(C);
}

int main() {
    test_basic_ops();
    test_from_file();
    test_large_matrix();
    printf("\n========== 矩阵运算全部验证完成 ==========\n");
    return 0;
}
