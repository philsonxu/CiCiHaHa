/**
 * @file kmeans.c - K-Means聚类
 */
#include "ml_models.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

KMeans* kmeans_create(size_t k, size_t feat_dim, int max_iters, float tol) {
    KMeans* m = (KMeans*)calloc(1,sizeof(KMeans));
    m->k = k; m->feat_dim = feat_dim;
    m->centroids = mat_create(k, feat_dim);
    m->assignments = NULL;
    m->max_iters = max_iters; m->tol = tol;
    return m;
}
void kmeans_free(KMeans* m) {
    if (m) { mat_free(&m->centroids); free(m->assignments); free(m); }
}

int kmeans_fit(KMeans* m, const Dataset* ds, float* inertia) {
    size_t n = ds->n, d = m->feat_dim, k = m->k;
    m->assignments = (int*)realloc(m->assignments, n*sizeof(int));
    /* k-means++ 初始化 */
    /* 随机选第一个中心 */
    size_t first = (size_t)(random_uniform(0,1)*n) % n;
    memcpy(mat_at(&m->centroids,0,0), mat_at_c(&ds->X,first,0), d*sizeof(float));
    float* dists = (float*)malloc(n*sizeof(float));
    for (size_t ci = 1; ci < k; ci++) {
        /* 计算到已选中心的最近距离平方 */
        for (size_t i = 0; i < n; i++) {
            float best = 1e30f;
            for (size_t j = 0; j < ci; j++) {
                float s = 0;
                for (size_t f=0;f<d;f++){float t=*mat_at_c(&ds->X,i,f)-*mat_at_c(&m->centroids,j,f);s+=t*t;}
                if (s<best) best=s;
            }
            dists[i]=best;
        }
        /* 按距离平方加权随机选下一个 */
        float total = vec_sum(dists, n);
        float r = random_uniform(0, total);
        size_t sel = n-1; float acc = 0;
        for (size_t i=0;i<n;i++) { acc += dists[i]; if (acc>=r) {sel=i;break;} }
        memcpy(mat_at(&m->centroids,ci,0), mat_at_c(&ds->X,sel,0), d*sizeof(float));
    }
    free(dists);

    int it;
    float prev_inertia = 1e30f;
    for (it = 0; it < m->max_iters; it++) {
        /* Assign */
        float iner = 0;
        for (size_t i=0;i<n;i++) {
            float best=1e30f; size_t bc=0;
            for (size_t c=0;c<k;c++) {
                float s=0;
                for (size_t f=0;f<d;f++){float t=*mat_at_c(&ds->X,i,f)-*mat_at_c(&m->centroids,c,f);s+=t*t;}
                if (s<best){best=s;bc=c;}
            }
            m->assignments[i]=(int)bc; iner += best;
        }
        /* Update */
        float* new_c = (float*)calloc(k*d, sizeof(float));
        size_t* cnt = (size_t*)calloc(k, sizeof(size_t));
        for (size_t i=0;i<n;i++) {
            int c = m->assignments[i]; cnt[c]++;
            for (size_t f=0;f<d;f++) new_c[c*d+f] += *mat_at_c(&ds->X,i,f);
        }
        for (size_t c=0;c<k;c++) {
            if (cnt[c] == 0) {
                /* 空簇: 重新随机化到一个随机样本 */
                size_t ri = (size_t)(random_uniform(0,1)*n)%n;
                memcpy(mat_at(&m->centroids,c,0), mat_at_c(&ds->X,ri,0), d*sizeof(float));
            } else {
                for (size_t f=0;f<d;f++)
                    *mat_at(&m->centroids,c,f) = new_c[c*d+f]/cnt[c];
            }
        }
        free(new_c); free(cnt);
        if (fabsf(iner-prev_inertia) < m->tol) { it++; break; }
        prev_inertia = iner;
    }
    if (inertia) {
        float iner=0;
        for (size_t i=0;i<n;i++){int c=m->assignments[i];float s=0;
            for(size_t f=0;f<d;f++){float t=*mat_at_c(&ds->X,i,f)-*mat_at_c(&m->centroids,c,f);s+=t*t;}iner+=s;}
        *inertia = iner;
    }
    return it;
}

int kmeans_predict_one(const KMeans* m, const float* x) {
    float best=1e30f; size_t bc=0;
    for (size_t c=0;c<m->k;c++){
        float s=0;
        for (size_t f=0;f<m->feat_dim;f++){float t=x[f]-*mat_at_c(&m->centroids,c,f);s+=t*t;}
        if (s<best){best=s;bc=c;}
    }
    return (int)bc;
}
