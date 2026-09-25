/**
 * @file matrix_ops.c
 * @brief 矩阵基础运算实现
 */
#include "matrix_ops.h"

Matrix* mat_add(const Matrix *a, const Matrix *b) {
    if (a->rows != b->rows || a->cols != b->cols) return NULL;
    Matrix *r = mat_create(a->rows, a->cols);
    for (int i = 0; i < a->rows; i++)
        for (int j = 0; j < a->cols; j++)
            r->data[i][j] = a->data[i][j] + b->data[i][j];
    return r;
}

Matrix* mat_sub(const Matrix *a, const Matrix *b) {
    if (a->rows != b->rows || a->cols != b->cols) return NULL;
    Matrix *r = mat_create(a->rows, a->cols);
    for (int i = 0; i < a->rows; i++)
        for (int j = 0; j < a->cols; j++)
            r->data[i][j] = a->data[i][j] - b->data[i][j];
    return r;
}

Matrix* mat_mul(const Matrix *a, const Matrix *b) {
    if (a->cols != b->rows) return NULL;
    Matrix *r = mat_create(a->rows, b->cols);
    for (int i = 0; i < a->rows; i++)
        for (int k = 0; k < a->cols; k++) {
            double aik = a->data[i][k];
            for (int j = 0; j < b->cols; j++)
                r->data[i][j] += aik * b->data[k][j];
        }
    return r;
}

Matrix* mat_transpose(const Matrix *a) {
    Matrix *r = mat_create(a->cols, a->rows);
    for (int i = 0; i < a->rows; i++)
        for (int j = 0; j < a->cols; j++)
            r->data[j][i] = a->data[i][j];
    return r;
}

/** @brief 交换矩阵两行 */
static void swap_rows(Matrix *m, int i, int j) {
    double *tmp = m->data[i];
    m->data[i] = m->data[j];
    m->data[j] = tmp;
}

/** @brief 交换向量两行（用于置换向量） */
static void swap_pivots(int *p, int i, int j) {
    int tmp = p[i]; p[i] = p[j]; p[j] = tmp;
}

int mat_lu_decomp(const Matrix *A, Matrix *L, Matrix *U, Matrix *P) {
    int n = A->rows;
    // 初始化 U = A副本，L = 单位矩阵，P = 单位矩阵
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            U->data[i][j] = A->data[i][j];
            L->data[i][j] = (i == j) ? 1.0 : 0.0;
            P->data[i][j] = (i == j) ? 1.0 : 0.0;
        }
    }
    int *piv = (int*)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) piv[i] = i;

    for (int k = 0; k < n; k++) {
        // 部分选主元
        int max_row = k;
        double max_val = fabs(U->data[k][k]);
        for (int i = k + 1; i < n; i++) {
            if (fabs(U->data[i][k]) > max_val) {
                max_val = fabs(U->data[i][k]);
                max_row = i;
            }
        }
        if (max_val < EPS) { free(piv); return -1; } // 奇异
        if (max_row != k) {
            swap_rows(U, k, max_row);
            swap_pivots(piv, k, max_row);
            // 交换L中已计算的前k列
            for (int j = 0; j < k; j++) {
                double t = L->data[k][j];
                L->data[k][j] = L->data[max_row][j];
                L->data[max_row][j] = t;
            }
        }
        // 高斯消元
        for (int i = k + 1; i < n; i++) {
            L->data[i][k] = U->data[i][k] / U->data[k][k];
            for (int j = k; j < n; j++)
                U->data[i][j] -= L->data[i][k] * U->data[k][j];
        }
    }
    // 构建置换矩阵P
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            P->data[i][j] = (piv[i] == j) ? 1.0 : 0.0;
    free(piv);
    return 0;
}

double mat_det(const Matrix *A) {
    int n = A->rows;
    Matrix *L = mat_create(n, n);
    Matrix *U = mat_create(n, n);
    Matrix *P = mat_create(n, n);
    int sign = 1; // 置换符号
    // 简化行列式：直接带符号连乘U对角线
    Matrix *Acopy = mat_copy(A);
    for (int k = 0; k < n; k++) {
        int max_row = k;
        double max_val = fabs(Acopy->data[k][k]);
        for (int i = k + 1; i < n; i++)
            if (fabs(Acopy->data[i][k]) > max_val) { max_val = fabs(Acopy->data[i][k]); max_row = i; }
        if (max_val < EPS) { mat_free(L); mat_free(U); mat_free(P); mat_free(Acopy); return 0.0; }
        if (max_row != k) { swap_rows(Acopy, k, max_row); sign = -sign; }
        for (int i = k + 1; i < n; i++) {
            double factor = Acopy->data[i][k] / Acopy->data[k][k];
            for (int j = k; j < n; j++)
                Acopy->data[i][j] -= factor * Acopy->data[k][j];
        }
    }
    double det = sign;
    for (int i = 0; i < n; i++) det *= Acopy->data[i][i];
    mat_free(L); mat_free(U); mat_free(P); mat_free(Acopy);
    return det;
}

/** @brief 前代解 Ly=Pb */
static void forward_sub(const Matrix *L, const Vector *b, Vector *y) {
    int n = L->rows;
    for (int i = 0; i < n; i++) {
        y->data[i] = b->data[i];
        for (int j = 0; j < i; j++) y->data[i] -= L->data[i][j] * y->data[j];
        y->data[i] /= L->data[i][i];
    }
}

/** @brief 回代解 Ux=y */
static void back_sub(const Matrix *U, const Vector *y, Vector *x) {
    int n = U->rows;
    for (int i = n - 1; i >= 0; i--) {
        x->data[i] = y->data[i];
        for (int j = i + 1; j < n; j++) x->data[i] -= U->data[i][j] * x->data[j];
        x->data[i] /= U->data[i][i];
    }
}

Matrix* mat_inverse(const Matrix *A) {
    int n = A->rows;
    Matrix *L = mat_create(n, n);
    Matrix *U = mat_create(n, n);
    Matrix *P = mat_create(n, n);
    if (mat_lu_decomp(A, L, U, P) != 0) { mat_free(L); mat_free(U); mat_free(P); return NULL; }

    Matrix *inv = mat_create(n, n);
    Vector *e = vec_create(n);
    Vector *Pb = vec_create(n);
    Vector *y = vec_create(n);
    Vector *x = vec_create(n);

    // 对单位向量逐个求解 A^{-1} * e_j = col_j
    for (int j = 0; j < n; j++) {
        for (int i = 0; i < n; i++) e->data[i] = (i == j) ? 1.0 : 0.0;
        // Pb = P * e
        for (int i = 0; i < n; i++) {
            Pb->data[i] = 0;
            for (int k = 0; k < n; k++) Pb->data[i] += P->data[i][k] * e->data[k];
        }
        forward_sub(L, Pb, y);
        back_sub(U, y, x);
        for (int i = 0; i < n; i++) inv->data[i][j] = x->data[i];
    }
    mat_free(L); mat_free(U); mat_free(P);
    vec_free(e); vec_free(Pb); vec_free(y); vec_free(x);
    return inv;
}
