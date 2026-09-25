/**
 * @file naive_bayes.c - 高斯朴素贝叶斯
 */
#include "ml_models.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

GaussianNB* gnb_create(size_t feat_dim, size_t num_classes, float var_smoothing) {
    GaussianNB* m = (GaussianNB*)calloc(1,sizeof(GaussianNB));
    m->feat_dim = feat_dim; m->num_classes = num_classes;
    m->class_prior = (float*)calloc(num_classes, sizeof(float));
    m->theta = (float*)calloc(num_classes*feat_dim, sizeof(float));
    m->sigma = (float*)calloc(num_classes*feat_dim, sizeof(float));
    m->var_smoothing = var_smoothing;
    return m;
}
void gnb_free(GaussianNB* m) {
    if (m) { free(m->class_prior); free(m->theta); free(m->sigma); free(m); }
}

void gnb_train(GaussianNB* m, const Dataset* ds) {
    size_t n = ds->n, d = m->feat_dim, C = m->num_classes;
    size_t* cnt = (size_t*)calloc(C, sizeof(size_t));
    vec_fill(m->theta, 0, C*d);
    /* 先算均值 */
    for (size_t i=0;i<n;i++) {
        int c = ds->labels[i]; cnt[c]++;
        for (size_t f=0;f<d;f++) m->theta[c*d+f] += *mat_at_c(&ds->X,i,f);
    }
    float max_var = 0;
    for (size_t c=0;c<C;c++) {
        m->class_prior[c] = (float)cnt[c]/n;
        if (cnt[c] > 0)
            for (size_t f=0;f<d;f++) m->theta[c*d+f] /= cnt[c];
    }
    /* 方差 */
    vec_fill(m->sigma, 0, C*d);
    for (size_t i=0;i<n;i++) {
        int c = ds->labels[i];
        for (size_t f=0;f<d;f++) {
            float t = *mat_at_c(&ds->X,i,f) - m->theta[c*d+f];
            m->sigma[c*d+f] += t*t;
        }
    }
    for (size_t c=0;c<C;c++) {
        for (size_t f=0;f<d;f++) {
            if (cnt[c] > 1) m->sigma[c*d+f] /= cnt[c];
            else m->sigma[c*d+f] = 0;
            if (m->sigma[c*d+f] > max_var) max_var = m->sigma[c*d+f];
        }
    }
    /* var smoothing */
    for (size_t c=0;c<C;c++)
        for (size_t f=0;f<d;f++) m->sigma[c*d+f] += m->var_smoothing*max_var;
    free(cnt);
}

int gnb_predict_one(const GaussianNB* m, const float* x) {
    size_t d = m->feat_dim, C = m->num_classes;
    float best = -1e30f; int bc = 0;
    for (size_t c=0;c<C;c++) {
        float lp = logf(m->class_prior[c] + 1e-12f);
        for (size_t f=0;f<d;f++) {
            float mu = m->theta[c*d+f];
            float var = m->sigma[c*d+f];
            lp += -0.5f*logf(2*3.1415926f*var) - 0.5f*(x[f]-mu)*(x[f]-mu)/var;
        }
        if (lp > best) { best = lp; bc = (int)c; }
    }
    return bc;
}
void gnb_predict(const GaussianNB* m, const Dataset* t, int* pred) {
    for(size_t i=0;i<t->n;i++) pred[i]=gnb_predict_one(m,(const float*)mat_at_c(&t->X,i,0));
}
