/**
 * @file regression.c - 线性/逻辑/Softmax回归
 */
#include "ml_models.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <assert.h>

/* ========== 线性回归 ========== */
LinearRegression* lr_create(size_t feat_dim, float lr, int epochs) {
    LinearRegression* m = (LinearRegression*)calloc(1,sizeof(LinearRegression));
    m->feat_dim = feat_dim;
    m->w = (float*)calloc(feat_dim, sizeof(float));
    m->b = 0; m->lr = lr; m->epochs = epochs;
    return m;
}
void lr_free(LinearRegression* m) { if(m){free(m->w);free(m);} }

float lr_predict_one(const LinearRegression* m, const float* x) {
    return vec_dot(m->w, x, m->feat_dim) + m->b;
}
void lr_predict(const LinearRegression* m, const Dataset* test, float* pred) {
    for (size_t i=0;i<test->n;i++) pred[i] = lr_predict_one(m,(const float*)mat_at_c(&test->X,i,0));
}

void lr_train_gd(LinearRegression* m, const Dataset* ds) {
    size_t n = ds->n, d = m->feat_dim;
    float* grad_w = (float*)calloc(d, sizeof(float));
    for (int ep = 0; ep < m->epochs; ep++) {
        float grad_b = 0; vec_fill(grad_w,0,d);
        float total_loss = 0;
        for (size_t i = 0; i < n; i++) {
            const float* x = (const float*)mat_at_c(&ds->X,i,0);
            float y = *mat_at_c(&ds->y,i,0);
            float pred = lr_predict_one(m, x);
            float err = pred - y;
            total_loss += err*err;
            for (size_t j=0;j<d;j++) grad_w[j] += err * x[j];
            grad_b += err;
        }
        for (size_t j=0;j<d;j++) m->w[j] -= m->lr * 2.0f/n * grad_w[j];
        m->b -= m->lr * 2.0f/n * grad_b;
        if (ep % max(1,m->epochs/10) == 0)
            printf("  LR GD epoch %d loss=%.6f\n", ep, total_loss/n);
    }
    free(grad_w);
}

/* 闭式解 w = (X^T X)^{-1} X^T y, 这里使用增广矩阵 [X 1] 解b */
void lr_train_closed(LinearRegression* m, const Dataset* ds) {
    size_t n = ds->n, d = m->feat_dim;
    size_t D = d+1;
    Matrix Xa = mat_create(n, D);
    Matrix XtX = mat_create(D, D);
    Matrix Xty = mat_create(D, 1);
    for (size_t i=0;i<n;i++) {
        memcpy(mat_at(&Xa,i,0), mat_at_c(&ds->X,i,0), d*sizeof(float));
        *mat_at(&Xa,i,d) = 1.0f;
    }
    /* XtX = Xa^T Xa */
    mat_transpose_mul(&Xa, &Xa, &XtX);
    /* Xty = Xa^T y */
    mat_transpose_mul(&Xa, &ds->y, &Xty);
    /* 高斯消元求逆+求解: Gauss-Jordan on [XtX|Xty] */
    for (size_t i=0;i<D;i++) {
        /* 选主元 */
        size_t pv = i; float pv_val = fabsf(*mat_at(&XtX,i,i));
        for (size_t r=i+1;r<D;r++)
            if (fabsf(*mat_at(&XtX,r,i)) > pv_val) { pv_val=fabsf(*mat_at(&XtX,r,i)); pv=r; }
        if (pv != i) {
            for (size_t c=0;c<D;c++) { float t=*mat_at(&XtX,i,c); *mat_at(&XtX,i,c)=*mat_at(&XtX,pv,c); *mat_at(&XtX,pv,c)=t; }
            float t=*mat_at(&Xty,i,0); *mat_at(&Xty,i,0)=*mat_at(&Xty,pv,0); *mat_at(&Xty,pv,0)=t;
        }
        float piv = *mat_at(&XtX,i,i);
        for (size_t c=0;c<D;c++) *mat_at(&XtX,i,c) /= piv;
        *mat_at(&Xty,i,0) /= piv;
        for (size_t r=0;r<D;r++) {
            if (r==i) continue;
            float f = *mat_at(&XtX,r,i);
            for (size_t c=0;c<D;c++) *mat_at(&XtX,r,c) -= f * *mat_at(&XtX,i,c);
            *mat_at(&Xty,r,0) -= f * *mat_at(&Xty,i,0);
        }
    }
    for (size_t j=0;j<d;j++) m->w[j] = *mat_at(&Xty,j,0);
    m->b = *mat_at(&Xty,d,0);
    mat_free(&Xa); mat_free(&XtX); mat_free(&Xty);
}

/* ========== 逻辑回归 (二分类) ========== */
LogisticRegression* logreg_create(size_t feat_dim, float lr, float l2, int ep) {
    LogisticRegression* m = (LogisticRegression*)calloc(1,sizeof(LogisticRegression));
    m->feat_dim = feat_dim;
    m->w = (float*)calloc(feat_dim, sizeof(float));
    m->b = 0; m->lr=lr; m->l2_reg=l2; m->epochs=ep;
    return m;
}
void logreg_free(LogisticRegression* m){if(m){free(m->w);free(m);}}

float logreg_predict_proba(const LogisticRegression* m, const float* x) {
    float z = vec_dot(m->w,x,m->feat_dim) + m->b;
    return sigmoid(z);
}
int logreg_predict_one(const LogisticRegression* m, const float* x) {
    return logreg_predict_proba(m,x) > 0.5f ? 1 : 0;
}
void logreg_predict(const LogisticRegression* m, const Dataset* t, int* pred) {
    for (size_t i=0;i<t->n;i++) pred[i] = logreg_predict_one(m,(const float*)mat_at_c(&t->X,i,0));
}

void logreg_train(LogisticRegression* m, const Dataset* ds) {
    size_t n=ds->n, d=m->feat_dim;
    float* gw = (float*)calloc(d,sizeof(float));
    for (int ep=0;ep<m->epochs;ep++) {
        vec_fill(gw,0,d); float gb=0; float loss=0;
        for (size_t i=0;i<n;i++) {
            const float* x=(const float*)mat_at_c(&ds->X,i,0);
            float y=(float)ds->labels[i];
            float p = logreg_predict_proba(m,x);
            /* BCE损失 */
            loss += -(y*logf(p+1e-12f) + (1-y)*logf(1-p+1e-12f));
            float err = p - y;
            for (size_t j=0;j<d;j++) gw[j] += err*x[j];
            gb += err;
        }
        for (size_t j=0;j<d;j++) {
            gw[j] = gw[j]/n + m->l2_reg*m->w[j];
            m->w[j] -= m->lr*gw[j];
        }
        m->b -= m->lr*gb/n;
        if (ep % max(1,m->epochs/10) == 0)
            printf("  LogReg epoch %d loss=%.6f\n", ep, loss/n);
    }
    free(gw);
}

/* ========== Softmax 多分类逻辑回归 ========== */
SoftmaxRegression* sm_create(size_t feat_dim, size_t nc, float lr, float l2, int ep) {
    SoftmaxRegression* m = (SoftmaxRegression*)calloc(1,sizeof(SoftmaxRegression));
    m->feat_dim = feat_dim; m->num_classes = nc;
    m->W = mat_create(feat_dim, nc);
    m->b = (float*)calloc(nc,sizeof(float));
    init_xavier(&m->W, feat_dim, nc);
    m->lr=lr; m->l2_reg=l2; m->epochs=ep;
    return m;
}
void sm_free(SoftmaxRegression* m) {
    if (m) { mat_free(&m->W); free(m->b); free(m); }
}
void sm_predict_proba(const SoftmaxRegression* m, const float* x, float* proba) {
    for (size_t c=0;c<m->num_classes;c++) {
        proba[c] = m->b[c];
        for (size_t j=0;j<m->feat_dim;j++) proba[c] += x[j] * *mat_at_c(&m->W,j,c);
    }
    softmax_row(proba, m->num_classes);
}
int sm_predict_one(const SoftmaxRegression* m, const float* x) {
    float proba[32]; /* 足够小的类别数 */
    float* p = proba;
    float* buf = NULL;
    if (m->num_classes > 32) { buf=(float*)malloc(m->num_classes*sizeof(float)); p=buf; }
    sm_predict_proba(m,x,p);
    size_t mi; vec_max(p, m->num_classes, &mi);
    free(buf); return (int)mi;
}
void sm_predict(const SoftmaxRegression* m, const Dataset* t, int* pred) {
    for (size_t i=0;i<t->n;i++) pred[i]=sm_predict_one(m,(const float*)mat_at_c(&t->X,i,0));
}
void sm_train(SoftmaxRegression* m, const Dataset* ds) {
    size_t n=ds->n, d=m->feat_dim, C=m->num_classes;
    Matrix gW = mat_create(d, C);
    float* gb = (float*)calloc(C,sizeof(float));
    float* p = (float*)malloc(C*sizeof(float));
    for (int ep=0;ep<m->epochs;ep++) {
        mat_fill(&gW,0); vec_fill(gb,0,C); float loss=0;
        for (size_t i=0;i<n;i++) {
            const float* x=(const float*)mat_at_c(&ds->X,i,0);
            int y = ds->labels[i];
            sm_predict_proba(m,x,p);
            loss += -logf(p[y] + 1e-12f);
            for (size_t c=0;c<C;c++) {
                float err = p[c] - ((c==(size_t)y)?1.0f:0.0f);
                for (size_t j=0;j<d;j++) *mat_at(&gW,j,c) += err*x[j];
                gb[c] += err;
            }
        }
        for (size_t c=0;c<C;c++) {
            for (size_t j=0;j<d;j++) {
                float g = *mat_at(&gW,j,c)/n + m->l2_reg**mat_at_c(&m->W,j,c);
                *mat_at(&m->W,j,c) -= m->lr * g;
            }
            m->b[c] -= m->lr*gb[c]/n;
        }
        if (ep % max(1,m->epochs/10) == 0)
            printf("  Softmax epoch %d loss=%.6f\n", ep, loss/n);
    }
    mat_free(&gW); free(gb); free(p);
}
