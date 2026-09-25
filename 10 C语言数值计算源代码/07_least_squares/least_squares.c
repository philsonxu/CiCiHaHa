/**
 * @file least_squares.c
 * @brief 最小二乘拟合实现（通过正规方程 A^T A c = A^T y，内置高斯消元求解）
 */
#include "least_squares.h"

/** @brief 内置高斯消元（不依赖外部模块） */
static int _gauss_solve(const Matrix *A, const Vector *b, Vector *x) {
    int n = A->rows;
    Matrix *aug = mat_create(n, n+1);
    for (int i=0;i<n;i++){for(int j=0;j<n;j++)aug->data[i][j]=A->data[i][j]; aug->data[i][n]=b->data[i];}
    for(int k=0;k<n;k++){
        int mr=k; for(int i=k+1;i<n;i++)if(fabs(aug->data[i][k])>fabs(aug->data[mr][k]))mr=i;
        if(fabs(aug->data[mr][k])<1e-14){mat_free(aug);return -1;}
        if(mr!=k){double*t=aug->data[k];aug->data[k]=aug->data[mr];aug->data[mr]=t;}
        for(int i=k+1;i<n;i++){double f=aug->data[i][k]/aug->data[k][k];for(int j=k;j<=n;j++)aug->data[i][j]-=f*aug->data[k][j];}
    }
    for(int i=n-1;i>=0;i--){x->data[i]=aug->data[i][n];for(int j=i+1;j<n;j++)x->data[i]-=aug->data[i][j]*x->data[j];x->data[i]/=aug->data[i][i];}
    mat_free(aug); return 0;
}

void poly_fit(const double *x, const double *y, int n, int m, double *coeff) {
    int k = m + 1;
    Matrix *G = mat_create(k, k);
    Vector *b = vec_create(k);
    double *S = (double*)calloc(2*m+1, sizeof(double));
    double *T = (double*)calloc(m+1, sizeof(double));
    for (int p = 0; p < n; p++) {
        double xi = x[p], yi = y[p];
        double xp = 1.0;
        for (int i = 0; i <= 2*m; i++) { S[i] += xp; xp *= xi; }
        xp = 1.0;
        for (int i = 0; i <= m; i++) { T[i] += yi * xp; xp *= xi; }
    }
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < k; j++) G->data[i][j] = S[i+j];
        b->data[i] = T[i];
    }
    Vector *c = vec_create(k);
    _gauss_solve(G, b, c);
    for (int i = 0; i < k; i++) coeff[i] = c->data[i];
    mat_free(G); vec_free(b); vec_free(c);
    free(S); free(T);
}

double poly_eval(const double *coeff, int m, double x) {
    double s = coeff[m];
    for (int i = m-1; i >= 0; i--) s = s*x + coeff[i];
    return s;
}

double r_squared(const double *x, const double *y, int n, const double *coeff, int m) {
    double y_mean = 0;
    for (int i = 0; i < n; i++) y_mean += y[i];
    y_mean /= n;
    double ss_tot = 0, ss_res = 0;
    for (int i = 0; i < n; i++) {
        double yp = poly_eval(coeff, m, x[i]);
        ss_res += (y[i] - yp)*(y[i] - yp);
        ss_tot += (y[i] - y_mean)*(y[i] - y_mean);
    }
    return 1.0 - ss_res / ss_tot;
}
