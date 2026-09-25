/**
 * @file linear_solver.c
 * @brief 线性方程组求解实现
 */
#include "linear_solver.h"

int gauss_solve(const Matrix *A, const Vector *b, Vector *x) {
    int n = A->rows;
    // 构造增广矩阵
    Matrix *aug = mat_create(n, n + 1);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) aug->data[i][j] = A->data[i][j];
        aug->data[i][n] = b->data[i];
    }
    // 高斯消元（部分选主元）
    for (int k = 0; k < n; k++) {
        int max_row = k;
        double max_val = fabs(aug->data[k][k]);
        for (int i = k + 1; i < n; i++)
            if (fabs(aug->data[i][k]) > max_val) { max_val = fabs(aug->data[i][k]); max_row = i; }
        if (max_val < EPS) { mat_free(aug); return -1; }
        if (max_row != k) {
            double *tmp = aug->data[k]; aug->data[k] = aug->data[max_row]; aug->data[max_row] = tmp;
        }
        for (int i = k + 1; i < n; i++) {
            double factor = aug->data[i][k] / aug->data[k][k];
            for (int j = k; j <= n; j++) aug->data[i][j] -= factor * aug->data[k][j];
        }
    }
    // 回代
    for (int i = n - 1; i >= 0; i--) {
        x->data[i] = aug->data[i][n];
        for (int j = i + 1; j < n; j++) x->data[i] -= aug->data[i][j] * x->data[j];
        x->data[i] /= aug->data[i][i];
    }
    mat_free(aug);
    return 0;
}

int jacobi_solve(const Matrix *A, const Vector *b, Vector *x, double tol, int max_iter) {
    int n = A->rows;
    Vector *x_new = vec_create(n);
    int iter = 0;
    for (iter = 0; iter < max_iter; iter++) {
        for (int i = 0; i < n; i++) {
            double s = b->data[i];
            for (int j = 0; j < n; j++)
                if (j != i) s -= A->data[i][j] * x->data[j];
            x_new->data[i] = s / A->data[i][i];
        }
        // 收敛判断
        double err = vec_error_l2(x_new, x) / (vec_norm2(x_new) + EPS);
        for (int i = 0; i < n; i++) x->data[i] = x_new->data[i];
        if (err < tol) break;
    }
    vec_free(x_new);
    return iter + 1;
}

int gauss_seidel_solve(const Matrix *A, const Vector *b, Vector *x, double tol, int max_iter) {
    int n = A->rows;
    Vector *x_old = vec_create(n);
    int iter = 0;
    for (iter = 0; iter < max_iter; iter++) {
        for (int i = 0; i < n; i++) x_old->data[i] = x->data[i];
        for (int i = 0; i < n; i++) {
            double s = b->data[i];
            for (int j = 0; j < i; j++) s -= A->data[i][j] * x->data[j];
            for (int j = i + 1; j < n; j++) s -= A->data[i][j] * x->data[j];
            x->data[i] = s / A->data[i][i];
        }
        double err = vec_error_l2(x, x_old) / (vec_norm2(x) + EPS);
        if (err < tol) break;
    }
    vec_free(x_old);
    return iter + 1;
}

/** @brief 矩阵-向量乘法 y = A*x */
static void mat_vec_mul(const Matrix *A, const Vector *x, Vector *y) {
    for (int i = 0; i < A->rows; i++) {
        y->data[i] = 0;
        for (int j = 0; j < A->cols; j++) y->data[i] += A->data[i][j] * x->data[j];
    }
}

/** @brief 向量内积 */
static double vec_dot(const Vector *a, const Vector *b) {
    double s = 0;
    for (int i = 0; i < a->size; i++) s += a->data[i] * b->data[i];
    return s;
}

/** @brief 向量加法 y = a*x + y (axpy) */
static void vec_axpy(double a, const Vector *x, Vector *y) {
    for (int i = 0; i < x->size; i++) y->data[i] += a * x->data[i];
}

int cg_solve(const Matrix *A, const Vector *b, Vector *x, double tol, int max_iter) {
    int n = A->rows;
    Vector *r = vec_create(n);
    Vector *p = vec_create(n);
    Vector *Ap = vec_create(n);
    // r0 = b - A*x0
    mat_vec_mul(A, x, Ap);
    for (int i = 0; i < n; i++) { r->data[i] = b->data[i] - Ap->data[i]; p->data[i] = r->data[i]; }
    double rsold = vec_dot(r, r);
    double bnorm = vec_norm2(b);
    int iter = 0;
    for (iter = 0; iter < max_iter; iter++) {
        if (sqrt(rsold) / bnorm < tol) break;
        mat_vec_mul(A, p, Ap);
        double alpha = rsold / vec_dot(p, Ap);
        vec_axpy(alpha, p, x);
        vec_axpy(-alpha, Ap, r);
        double rsnew = vec_dot(r, r);
        double beta = rsnew / rsold;
        for (int i = 0; i < n; i++) p->data[i] = r->data[i] + beta * p->data[i];
        rsold = rsnew;
    }
    vec_free(r); vec_free(p); vec_free(Ap);
    return iter + 1;
}
