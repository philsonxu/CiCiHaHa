/**
 * @file knn.c - K近邻算法 (欧氏距离 + 多数表决)
 */
#include "ml_models.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

KNN* knn_create(int k) {
    KNN* m = (KNN*)calloc(1,sizeof(KNN));
    m->k = k; m->train = NULL;
    return m;
}
void knn_train(KNN* model, const Dataset* ds) { model->train = ds; }

void knn_free(KNN* m) { free(m); }

int knn_predict_one(const KNN* model, const float* x) {
    const Dataset* tr = model->train;
    int k = model->k;
    size_t n = tr->n, d = tr->feat_dim;
    /* 存储 (距离, label) */
    float* dists = (float*)malloc(n*sizeof(float));
    for (size_t i = 0; i < n; i++) {
        float s = 0;
        const float* row = (const float*)mat_at_c(&tr->X, i, 0);
        for (size_t j = 0; j < d; j++) { float t=x[j]-row[j]; s += t*t; }
        dists[i] = s; /* 平方距离即可 */
    }
    /* 投票表 */
    int* vote = (int*)calloc(tr->num_classes, sizeof(int));
    /* 找k个最近邻: 简单方法, 每次找最小并标记 */
    int* used = (int*)calloc(n, sizeof(int));
    for (int ki = 0; ki < k; ki++) {
        float best = 1e30f; size_t bi = 0;
        for (size_t i = 0; i < n; i++)
            if (!used[i] && dists[i] < best) { best = dists[i]; bi = i; }
        used[bi] = 1;
        vote[tr->labels[bi]]++;
    }
    int best_lab = 0; int bv = vote[0];
    for (size_t c = 1; c < tr->num_classes; c++)
        if (vote[c] > bv) { bv = vote[c]; best_lab = (int)c; }
    free(dists); free(vote); free(used);
    return best_lab;
}

void knn_predict(const KNN* model, const Dataset* test, int* pred) {
    for (size_t i = 0; i < test->n; i++) {
        pred[i] = knn_predict_one(model, (const float*)mat_at_c(&test->X,i,0));
    }
}
