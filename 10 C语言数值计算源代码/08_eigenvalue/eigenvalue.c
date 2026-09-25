/**
 * @file eigenvalue.c
 * @brief 特征值算法实现
 */
#include "eigenvalue.h"

static void mat_vec(const Matrix *A, const double *x, double *y) {
    int n = A->rows;
    for (int i = 0; i < n; i++) {
        y[i] = 0;
        for (int j = 0; j < n; j++) y[i] += A->data[i][j] * x[j];
    }
}

static double vec_norm(double *x, int n) {
    double s = 0; for (int i=0;i<n;i++) s+=x[i]*x[i]; return sqrt(s);
}

double power_iteration(const Matrix *A, double *eigenvec, double tol, int max_iter, int *iter) {
    int n = A->rows;
    double *y = (double*)malloc(n*sizeof(double));
    // 初始向量 [1,1,...,1]
    for (int i=0;i<n;i++) eigenvec[i] = 1.0;
    double lambda = 0;
    for (*iter=0; *iter<max_iter; (*iter)++) {
        mat_vec(A, eigenvec, y);
        double nrm = vec_norm(y, n);
        for (int i=0;i<n;i++) eigenvec[i] = y[i]/nrm;
        double new_lambda = nrm;
        if (fabs(new_lambda - lambda) < tol) { lambda = new_lambda; break; }
        lambda = new_lambda;
    }
    free(y);
    return lambda;
}

/** 内置高斯消元（带选主元）用于反幂法解线性方程组 */
static void _solve_linear(Matrix *A, double *b, double *x, int n) {
    for (int k=0;k<n;k++){
        int mr=k;
        for(int i=k+1;i<n;i++)if(fabs(A->data[i][k])>fabs(A->data[mr][k]))mr=i;
        if(mr!=k){for(int j=k;j<n;j++){double t=A->data[k][j];A->data[k][j]=A->data[mr][j];A->data[mr][j]=t;} double t=b[k];b[k]=b[mr];b[mr]=t;}
        for(int i=k+1;i<n;i++){double f=A->data[i][k]/A->data[k][k];for(int j=k;j<n;j++)A->data[i][j]-=f*A->data[k][j];b[i]-=f*b[k];}
    }
    for(int i=n-1;i>=0;i--){x[i]=b[i];for(int j=i+1;j<n;j++)x[i]-=A->data[i][j]*x[j];x[i]/=A->data[i][i];}
}

double inverse_iteration(const Matrix *A, double *eigenvec, double tol, int max_iter, int *iter) {
    int n = A->rows;
    double *y = (double*)malloc(n*sizeof(double));
    double *b = (double*)malloc(n*sizeof(double));
    Matrix *LU = mat_create(n, n);
    for (int i=0;i<n;i++) eigenvec[i] = 1.0;
    double lambda = 0;
    for (*iter=0;*iter<max_iter;(*iter)++) {
        // 复制A到LU
        for(int i=0;i<n;i++)for(int j=0;j<n;j++)LU->data[i][j]=A->data[i][j];
        for(int i=0;i<n;i++) b[i] = eigenvec[i];
        _solve_linear(LU, b, y, n);
        double nrm = vec_norm(y, n);
        for(int i=0;i<n;i++) eigenvec[i] = y[i]/nrm;
        double new_lambda = 1.0/nrm; // 最小模特征值
        if (fabs(new_lambda - lambda) < tol && *iter>0) { lambda = new_lambda; break; }
        lambda = new_lambda;
    }
    mat_free(LU); free(y); free(b);
    return lambda;
}

void jacobi_eigen(const Matrix *A, double *eigenvalues, Matrix *V, double tol) {
    int n = A->rows;
    Matrix *B = mat_copy(A);
    // V = I
    for (int i=0;i<n;i++)for(int j=0;j<n;j++)V->data[i][j]=(i==j)?1.0:0;

    int max_sweeps = 100;
    for (int sweep=0; sweep<max_sweeps; sweep++) {
        // 找最大非对角元
        double max_off = 0;
        int p=0, q=1;
        for (int i=0;i<n;i++)for(int j=i+1;j<n;j++)
            if(fabs(B->data[i][j])>max_off){max_off=fabs(B->data[i][j]);p=i;q=j;}
        if(max_off<tol) break;
        // 计算旋转角theta
        double app=B->data[p][p], aqq=B->data[q][q], apq=B->data[p][q];
        double theta;
        if(fabs(app-aqq)<1e-15) theta=M_PI/4;
        else theta = 0.5*atan2(2*apq, aqq-app);
        double c=cos(theta), s=sin(theta);
        // 旋转更新B = G^T B G
        B->data[p][p] = c*c*app - 2*s*c*apq + s*s*aqq;
        B->data[q][q] = s*s*app + 2*s*c*apq + c*c*aqq;
        B->data[p][q] = B->data[q][p] = 0;
        for(int i=0;i<n;i++){
            if(i!=p&&i!=q){
                double aip=B->data[i][p], aiq=B->data[i][q];
                B->data[i][p]=B->data[p][i]=c*aip-s*aiq;
                B->data[i][q]=B->data[q][i]=s*aip+c*aiq;
            }
        }
        // 更新特征向量矩阵V = V G
        for(int i=0;i<n;i++){
            double vip=V->data[i][p], viq=V->data[i][q];
            V->data[i][p] = c*vip - s*viq;
            V->data[i][q] = s*vip + c*viq;
        }
    }
    for(int i=0;i<n;i++) eigenvalues[i] = B->data[i][i];
    mat_free(B);
}
