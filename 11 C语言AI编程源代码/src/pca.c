/**
 * @file pca.c - 主成分分析 (幂法 + deflation)
 */
#include "ml_models.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <assert.h>

PCA* pca_create(size_t n_components) {
    PCA* m = (PCA*)calloc(1,sizeof(PCA));
    m->n_components = n_components;
    m->mean = NULL; m->components.data=NULL; m->explained_var=NULL;
    return m;
}
void pca_free(PCA* m) {
    if (!m) return;
    free(m->mean); mat_free(&m->components); free(m->explained_var); free(m);
}

/* 幂法求协方差矩阵 C(dxd) 的最大特征值/向量 */
static float _power_iter(const Matrix* C, float* eig_vec, size_t d, int max_iters, float tol) {
    float* v = eig_vec;
    /* 随机初始化 */
    for (size_t i=0;i<d;i++) v[i] = random_uniform(-1,1);
    float n = vec_l2_norm(v,d); vec_mul_scalar(v, 1.0f/n, v, d);
    float* v_new = (float*)malloc(d*sizeof(float));
    float eig_val = 0;
    for (int it=0;it<max_iters;it++) {
        /* v_new = C v */
        vec_fill(v_new,0,d);
        for (size_t i=0;i<d;i++)
            for (size_t j=0;j<d;j++)
                v_new[i] += *mat_at_c(C,i,j) * v[j];
        float nv = vec_l2_norm(v_new,d);
        vec_mul_scalar(v_new, 1.0f/nv, v_new, d);
        /* 收敛检查 */
        float dot = fabsf(vec_dot(v, v_new, d));
        vec_copy(v_new, v, d);
        eig_val = nv;
        if (1-dot < tol) break;
    }
    /* Rayleigh商 = v^T C v */
    vec_fill(v_new,0,d);
    for (size_t i=0;i<d;i++) for (size_t j=0;j<d;j++) v_new[i]+=*mat_at_c(C,i,j)*v[j];
    float lam = vec_dot(v, v_new, d);
    free(v_new);
    return lam;
}

void pca_fit(PCA* m, const Dataset* ds) {
    size_t n = ds->n, d = ds->feat_dim;
    size_t k = m->n_components;
    m->feat_dim = d;
    m->components = mat_create(k, d);
    m->mean = (float*)calloc(d, sizeof(float));
    m->explained_var = (float*)calloc(k, sizeof(float));

    /* 中心化 */
    dataset_compute_mean_std(ds, m->mean, NULL);
    Matrix Xc = mat_create(n, d);
    for (size_t i=0;i<n;i++)
        for (size_t j=0;j<d;j++)
            *mat_at(&Xc,i,j) = *mat_at_c(&ds->X,i,j) - m->mean[j];
    /* 协方差矩阵 C = Xc^T Xc / (n-1) */
    Matrix C = mat_create(d, d);
    mat_transpose_mul(&Xc, &Xc, &C);
    mat_scale(&C, 1.0f/(n-1));
    mat_free(&Xc);

    /* 幂法+Deflation求前k个 */
    for (size_t c=0;c<k;c++) {
        float* v = (float*)mat_at(&m->components, c, 0);
        float lam = _power_iter(&C, v, d, 1000, 1e-7f);
        m->explained_var[c] = lam;
        /* Deflation: C = C - λ v v^T */
        for (size_t i=0;i<d;i++)
            for (size_t j=0;j<d;j++)
                *mat_at(&C,i,j) -= lam * v[i]*v[j];
    }
    mat_free(&C);
}

void pca_transform(const PCA* m, const Matrix* X, Matrix* X_proj) {
    size_t n = X->rows, d = X->cols;
    size_t k = m->n_components;
    assert(X_proj->rows == n && X_proj->cols == k);
    for (size_t i=0;i<n;i++) {
        for (size_t c=0;c<k;c++) {
            float s=0;
            for (size_t j=0;j<d;j++)
                s += (*mat_at_c(X,i,j) - m->mean[j]) * *mat_at_c(&m->components,c,j);
            *mat_at(X_proj,i,c) = s;
        }
    }
}
