/**
 * @file dataset.c - 数据集加载与生成
 */
#include "dataset.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <assert.h>

Dataset* dataset_create(size_t n, size_t feat_dim, size_t num_classes) {
    Dataset* ds = (Dataset*)calloc(1, sizeof(Dataset));
    ds->n = n; ds->feat_dim = feat_dim; ds->num_classes = num_classes;
    ds->X = mat_create(n, feat_dim);
    if (num_classes >= 2) {
        ds->y = mat_create(n, num_classes);
        ds->labels = (int*)calloc(n, sizeof(int));
    } else {
        ds->y = mat_create(n, 1);
        ds->labels = NULL;
    }
    return ds;
}
void dataset_free(Dataset* ds) {
    if (!ds) return;
    mat_free(&ds->X); mat_free(&ds->y);
    free(ds->labels); free(ds);
}

void dataset_shuffle(Dataset* ds) {
    size_t n = ds->n;
    for (size_t i = n-1; i > 0; i--) {
        size_t j = (size_t)(random_uniform(0,1) * (i+1));
        if (j > i) j = i;
        /* 交换行i,j */
        for (size_t k = 0; k < ds->feat_dim; k++) {
            float t = *mat_at(&ds->X, i, k);
            *mat_at(&ds->X, i, k) = *mat_at(&ds->X, j, k);
            *mat_at(&ds->X, j, k) = t;
        }
        for (size_t k = 0; k < ds->y.cols; k++) {
            float t = *mat_at(&ds->y, i, k);
            *mat_at(&ds->y, i, k) = *mat_at(&ds->y, j, k);
            *mat_at(&ds->y, j, k) = t;
        }
        if (ds->labels) { int t=ds->labels[i]; ds->labels[i]=ds->labels[j]; ds->labels[j]=t; }
    }
}

Dataset* dataset_split(Dataset* ds, float train_ratio, Dataset** test_ds) {
    size_t n_train = (size_t)(ds->n * train_ratio);
    size_t n_test = ds->n - n_train;
    Dataset* train = dataset_create(n_train, ds->feat_dim, ds->num_classes);
    Dataset* test = dataset_create(n_test, ds->feat_dim, ds->num_classes);
    for (size_t i = 0; i < n_train; i++) {
        memcpy(mat_at(&train->X,i,0), mat_at_c(&ds->X,i,0), ds->feat_dim*sizeof(float));
        memcpy(mat_at(&train->y,i,0), mat_at_c(&ds->y,i,0), ds->y.cols*sizeof(float));
        if (ds->labels) train->labels[i] = ds->labels[i];
    }
    for (size_t i = 0; i < n_test; i++) {
        size_t si = n_train + i;
        memcpy(mat_at(&test->X,i,0), mat_at_c(&ds->X,si,0), ds->feat_dim*sizeof(float));
        memcpy(mat_at(&test->y,i,0), mat_at_c(&ds->y,si,0), ds->y.cols*sizeof(float));
        if (ds->labels) test->labels[i] = ds->labels[si];
    }
    *test_ds = test;
    return train;
}

void dataset_compute_mean_std(const Dataset* ds, float* mean, float* std) {
    size_t n = ds->n, d = ds->feat_dim;
    vec_fill(mean, 0, d);
    for (size_t i = 0; i < n; i++)
        for (size_t j = 0; j < d; j++) mean[j] += *mat_at_c(&ds->X,i,j);
    for (size_t j = 0; j < d; j++) mean[j] /= n;
    if (!std) return;
    vec_fill(std, 0, d);
    for (size_t i = 0; i < n; i++)
        for (size_t j = 0; j < d; j++) {
            float v = *mat_at_c(&ds->X,i,j) - mean[j];
            std[j] += v*v;
        }
    for (size_t j = 0; j < d; j++) std[j] = sqrtf(std[j]/n) + 1e-8f;
}

void dataset_normalize(Dataset* ds, const float* mean, const float* std) {
    for (size_t i = 0; i < ds->n; i++)
        for (size_t j = 0; j < ds->feat_dim; j++)
            *mat_at(&ds->X,i,j) = (*mat_at_c(&ds->X,i,j) - mean[j]) / std[j];
}

float accuracy_score_labels(const int* pred, const int* true_label, size_t n) {
    size_t correct = 0;
    for (size_t i = 0; i < n; i++) if (pred[i] == true_label[i]) correct++;
    return (float)correct / n;
}

float accuracy_score(const Matrix* y_pred, const Matrix* y_true) {
    size_t n = y_pred->rows, c = y_pred->cols;
    size_t correct = 0;
    for (size_t i = 0; i < n; i++) {
        size_t pi=0, ti=0;
        vec_max(mat_at_c(y_pred,i,0), c, &pi);
        vec_max(mat_at_c(y_true,i,0), c, &ti);
        if (pi == ti) correct++;
    }
    return (float)correct / n;
}

float mse_score(const Matrix* y_pred, const Matrix* y_true) {
    size_t n = y_pred->rows, m = y_pred->cols;
    float s = 0;
    for (size_t i=0;i<n*m;i++) { float d = y_pred->data[i]-y_true->data[i]; s+=d*d; }
    return s/(n*m);
}

/* =========== Iris 内置数据集 (4特征,3分类,共150条) =========== */
/* 数据来源: Fisher经典Iris, 已打乱 */
static const float IRIS_DATA[150][5] = {
/* 花萼长,花萼宽,花瓣长,花瓣宽, 类别(0,1,2) */
{5.1f,3.5f,1.4f,0.2f,0},{4.9f,3.0f,1.4f,0.2f,0},{4.7f,3.2f,1.3f,0.2f,0},
{4.6f,3.1f,1.5f,0.2f,0},{5.0f,3.6f,1.4f,0.2f,0},{5.4f,3.9f,1.7f,0.4f,0},
{4.6f,3.4f,1.4f,0.3f,0},{5.0f,3.4f,1.5f,0.2f,0},{4.4f,2.9f,1.4f,0.2f,0},
{4.9f,3.1f,1.5f,0.1f,0},{5.4f,3.7f,1.5f,0.2f,0},{4.8f,3.4f,1.6f,0.2f,0},
{4.8f,3.0f,1.4f,0.1f,0},{4.3f,3.0f,1.1f,0.1f,0},{5.8f,4.0f,1.2f,0.2f,0},
{5.7f,4.4f,1.5f,0.4f,0},{5.4f,3.9f,1.3f,0.4f,0},{5.1f,3.5f,1.4f,0.3f,0},
{5.7f,3.8f,1.7f,0.3f,0},{5.1f,3.8f,1.5f,0.3f,0},{5.4f,3.4f,1.7f,0.2f,0},
{5.1f,3.7f,1.5f,0.4f,0},{4.6f,3.6f,1.0f,0.2f,0},{5.1f,3.3f,1.7f,0.5f,0},
{4.8f,3.4f,1.9f,0.2f,0},{5.0f,3.0f,1.6f,0.2f,0},{5.0f,3.4f,1.6f,0.4f,0},
{5.2f,3.5f,1.5f,0.2f,0},{5.2f,3.4f,1.4f,0.2f,0},{4.7f,3.2f,1.6f,0.2f,0},
{4.8f,3.1f,1.6f,0.2f,0},{5.4f,3.4f,1.5f,0.4f,0},{5.2f,4.1f,1.5f,0.1f,0},
{5.5f,4.2f,1.4f,0.2f,0},{4.9f,3.1f,1.5f,0.2f,0},{5.0f,3.2f,1.2f,0.2f,0},
{5.5f,3.5f,1.3f,0.2f,0},{4.9f,3.6f,1.4f,0.1f,0},{4.4f,3.0f,1.3f,0.2f,0},
{5.1f,3.4f,1.5f,0.2f,0},{5.0f,3.5f,1.3f,0.3f,0},{4.5f,2.3f,1.3f,0.3f,0},
{4.4f,3.2f,1.3f,0.2f,0},{5.0f,3.5f,1.6f,0.6f,0},{5.1f,3.8f,1.9f,0.4f,0},
{4.8f,3.0f,1.4f,0.3f,0},{5.1f,3.8f,1.6f,0.2f,0},{4.6f,3.2f,1.4f,0.2f,0},
{5.3f,3.7f,1.5f,0.2f,0},{5.0f,3.3f,1.4f,0.2f,0},
{7.0f,3.2f,4.7f,1.4f,1},{6.4f,3.2f,4.5f,1.5f,1},{6.9f,3.1f,4.9f,1.5f,1},
{5.5f,2.3f,4.0f,1.3f,1},{6.5f,2.8f,4.6f,1.5f,1},{5.7f,2.8f,4.5f,1.3f,1},
{6.3f,3.3f,4.7f,1.6f,1},{4.9f,2.4f,3.3f,1.0f,1},{6.6f,2.9f,4.6f,1.3f,1},
{5.2f,2.7f,3.9f,1.4f,1},{5.0f,2.0f,3.5f,1.0f,1},{5.9f,3.0f,4.2f,1.5f,1},
{6.0f,2.2f,4.0f,1.0f,1},{6.1f,2.9f,4.7f,1.4f,1},{5.6f,2.9f,3.6f,1.3f,1},
{6.7f,3.1f,4.4f,1.4f,1},{5.6f,3.0f,4.5f,1.5f,1},{5.8f,2.7f,4.1f,1.0f,1},
{6.2f,2.2f,4.5f,1.5f,1},{5.6f,2.5f,3.9f,1.1f,1},{5.9f,3.2f,4.8f,1.8f,1},
{6.1f,2.8f,4.0f,1.3f,1},{6.3f,2.5f,4.9f,1.5f,1},{6.1f,2.8f,4.7f,1.2f,1},
{6.4f,2.9f,4.3f,1.3f,1},{6.6f,3.0f,4.4f,1.4f,1},{6.8f,2.8f,4.8f,1.4f,1},
{6.7f,3.0f,5.0f,1.7f,1},{6.0f,2.9f,4.5f,1.5f,1},{5.7f,2.6f,3.5f,1.0f,1},
{5.5f,2.4f,3.8f,1.1f,1},{5.5f,2.4f,3.7f,1.0f,1},{5.8f,2.7f,3.9f,1.2f,1},
{6.0f,2.7f,5.1f,1.6f,1},{5.4f,3.0f,4.5f,1.5f,1},{6.0f,3.4f,4.5f,1.6f,1},
{6.7f,3.1f,4.7f,1.5f,1},{6.3f,2.3f,4.4f,1.3f,1},{5.6f,3.0f,4.1f,1.3f,1},
{5.5f,2.5f,4.0f,1.3f,1},{5.5f,2.6f,4.4f,1.2f,1},{6.1f,3.0f,4.6f,1.4f,1},
{5.8f,2.6f,4.0f,1.2f,1},{5.0f,2.3f,3.3f,1.0f,1},{5.6f,2.7f,4.2f,1.3f,1},
{5.7f,3.0f,4.2f,1.2f,1},{5.7f,2.9f,4.2f,1.3f,1},{6.2f,2.9f,4.3f,1.3f,1},
{5.1f,2.5f,3.0f,1.1f,1},{5.7f,2.8f,4.1f,1.3f,1},
{6.3f,3.3f,6.0f,2.5f,2},{5.8f,2.7f,5.1f,1.9f,2},{7.1f,3.0f,5.9f,2.1f,2},
{6.3f,2.9f,5.6f,1.8f,2},{6.5f,3.0f,5.8f,2.2f,2},{7.6f,3.0f,6.6f,2.1f,2},
{4.9f,2.5f,4.5f,1.7f,2},{7.3f,2.9f,6.3f,1.8f,2},{6.7f,2.5f,5.8f,1.8f,2},
{7.2f,3.6f,6.1f,2.5f,2},{6.5f,3.2f,5.1f,2.0f,2},{6.4f,2.7f,5.3f,1.9f,2},
{6.8f,3.0f,5.5f,2.1f,2},{5.7f,2.5f,5.0f,2.0f,2},{5.8f,2.8f,5.1f,2.4f,2},
{6.4f,3.2f,5.3f,2.3f,2},{6.5f,3.0f,5.5f,1.8f,2},{7.7f,3.8f,6.7f,2.2f,2},
{7.7f,2.6f,6.9f,2.3f,2},{6.0f,2.2f,5.0f,1.5f,2},{6.9f,3.2f,5.7f,2.3f,2},
{5.6f,2.8f,4.9f,2.0f,2},{7.7f,2.8f,6.7f,2.0f,2},{6.3f,2.7f,4.9f,1.8f,2},
{6.7f,3.3f,5.7f,2.1f,2},{7.2f,3.2f,6.0f,1.8f,2},{6.2f,2.8f,4.8f,1.8f,2},
{6.1f,3.0f,4.9f,1.8f,2},{6.4f,2.8f,5.6f,2.1f,2},{7.2f,3.0f,5.8f,1.6f,2},
{7.4f,2.8f,6.1f,1.9f,2},{7.9f,3.8f,6.4f,2.0f,2},{6.4f,2.8f,5.6f,2.2f,2},
{6.3f,2.8f,5.1f,1.5f,2},{6.1f,2.6f,5.6f,1.4f,2},{7.7f,3.0f,6.1f,2.3f,2},
{6.3f,3.4f,5.6f,2.4f,2},{6.4f,3.1f,5.5f,1.8f,2},{6.0f,3.0f,4.8f,1.8f,2},
{6.9f,3.1f,5.4f,2.1f,2},{6.7f,3.1f,5.6f,2.4f,2},{6.9f,3.1f,5.1f,2.3f,2},
{5.8f,2.7f,5.1f,1.9f,2},{6.8f,3.2f,5.9f,2.3f,2},{6.7f,3.3f,5.7f,2.5f,2},
{6.7f,3.0f,5.2f,2.3f,2},{6.3f,2.5f,5.0f,1.9f,2},{6.5f,3.0f,5.2f,2.0f,2},
{6.2f,3.4f,5.4f,2.3f,2},{5.9f,3.0f,5.1f,1.8f,2}
};

Dataset* dataset_load_iris(void) {
    Dataset* ds = dataset_create(150, 4, 3);
    for (size_t i = 0; i < 150; i++) {
        for (size_t j = 0; j < 4; j++)
            *mat_at(&ds->X, i, j) = IRIS_DATA[i][j];
        int lab = (int)IRIS_DATA[i][4];
        ds->labels[i] = lab;
        for (size_t k = 0; k < 3; k++)
            *mat_at(&ds->y, i, k) = (k == lab) ? 1.0f : 0.0f;
    }
    /* 标准化 */
    float mean[4], std[4];
    dataset_compute_mean_std(ds, mean, std);
    dataset_normalize(ds, mean, std);
    dataset_shuffle(ds);
    return ds;
}

/* =========== 合成数据集 =========== */
Dataset* dataset_make_linear(size_t n) {
    Dataset* ds = dataset_create(n, 2, 2);
    for (size_t i = 0; i < n; i++) {
        float x = random_uniform(-2,2), y = random_uniform(-2,2);
        int lab = (x + y > 0) ? 1 : 0;
        *mat_at(&ds->X,i,0)=x; *mat_at(&ds->X,i,1)=y;
        ds->labels[i] = lab;
        *mat_at(&ds->y,i,lab)=1;
    }
    return ds;
}

Dataset* dataset_make_moons(size_t n, float noise) {
    Dataset* ds = dataset_create(n, 2, 2);
    size_t n_out = n/2, n_in = n-n_out;
    for (size_t i = 0; i < n_out; i++) {
        float t = 3.14159f * random_uniform(0,1);
        float x = cosf(t) + random_normal(0,noise);
        float y = sinf(t) + random_normal(0,noise);
        *mat_at(&ds->X,i,0)=x; *mat_at(&ds->X,i,1)=y;
        ds->labels[i]=0; *mat_at(&ds->y,i,0)=1;
    }
    for (size_t i = 0; i < n_in; i++) {
        float t = 3.14159f * random_uniform(0,1);
        float x = 1 - cosf(t) + random_normal(0,noise);
        float y = 0.5f - sinf(t) + random_normal(0,noise);
        size_t si = n_out + i;
        *mat_at(&ds->X,si,0)=x; *mat_at(&ds->X,si,1)=y;
        ds->labels[si]=1; *mat_at(&ds->y,si,1)=1;
    }
    return ds;
}

Dataset* dataset_make_circles(size_t n, float noise) {
    Dataset* ds = dataset_create(n, 2, 2);
    size_t n_out = n/2, n_in = n-n_out;
    for (size_t i = 0; i < n_out; i++) {
        float t = 2*3.14159f*random_uniform(0,1);
        float r = 1.0f + random_normal(0,noise);
        *mat_at(&ds->X,i,0)=r*cosf(t); *mat_at(&ds->X,i,1)=r*sinf(t);
        ds->labels[i]=0; *mat_at(&ds->y,i,0)=1;
    }
    for (size_t i = 0; i < n_in; i++) {
        float t = 2*3.14159f*random_uniform(0,1);
        float r = 0.4f + random_normal(0,noise);
        size_t si = n_out+i;
        *mat_at(&ds->X,si,0)=r*cosf(t); *mat_at(&ds->X,si,1)=r*sinf(t);
        ds->labels[si]=1; *mat_at(&ds->y,si,1)=1;
    }
    return ds;
}

Dataset* dataset_make_xor(size_t n) {
    Dataset* ds = dataset_create(n, 2, 2);
    for (size_t i = 0; i < n; i++) {
        float x = random_uniform(-1,1);
        float y = random_uniform(-1,1);
        int lab = ((x>0) ^ (y>0)) ? 1 : 0;
        *mat_at(&ds->X,i,0)=x; *mat_at(&ds->X,i,1)=y;
        ds->labels[i]=lab; *mat_at(&ds->y,i,lab)=1;
    }
    return ds;
}

Dataset* dataset_make_regression(size_t n, size_t feat_dim, float noise) {
    Dataset* ds = dataset_create(n, feat_dim, 0); /* 回归无分类 */
    free(ds->labels); ds->labels=NULL;
    /* 随机权重 */
    float* w_true = (float*)malloc(feat_dim*sizeof(float));
    for (size_t j=0;j<feat_dim;j++) w_true[j] = random_uniform(-2,2);
    float b_true = random_uniform(-1,1);
    for (size_t i = 0; i < n; i++) {
        for (size_t j=0;j<feat_dim;j++) *mat_at(&ds->X,i,j)=random_uniform(-1,1);
        float y = b_true;
        for (size_t j=0;j<feat_dim;j++) y += w_true[j]*(*mat_at_c(&ds->X,i,j));
        y += random_normal(0, noise);
        *mat_at(&ds->y,i,0) = y;
    }
    free(w_true);
    return ds;
}

/* =========== MNIST IDX格式 =========== */
static int _read_uint32_be(FILE* f) {
    unsigned char b[4];
    if (fread(b,1,4,f)!=4) return -1;
    return (b[0]<<24)|(b[1]<<16)|(b[2]<<8)|b[3];
}
Dataset* dataset_load_mnist(const char* images_path, const char* labels_path, size_t max_n) {
    FILE* fi = fopen(images_path, "rb");
    FILE* fl = fopen(labels_path, "rb");
    if (!fi || !fl) {
        if (fi) fclose(fi); if (fl) fclose(fl);
        return NULL;
    }
    int magic_i = _read_uint32_be(fi);
    int n_img = _read_uint32_be(fi);
    int rows = _read_uint32_be(fi);
    int cols = _read_uint32_be(fi);
    int magic_l = _read_uint32_be(fl);
    int n_lab = _read_uint32_be(fl);
    if (magic_i != 2051 || magic_l != 2049 || rows != 28 || cols != 28) {
        fclose(fi); fclose(fl); return NULL;
    }
    (void)magic_i;(void)magic_l;
    size_t n = (size_t)((n_img < n_lab) ? n_img : n_lab);
    if (max_n > 0 && max_n < n) n = max_n;
    Dataset* ds = dataset_create(n, 28*28, 10);
    unsigned char* pix = (unsigned char*)malloc(28*28);
    unsigned char lab;
    for (size_t i = 0; i < n; i++) {
        fread(pix, 1, 28*28, fi);
        fread(&lab, 1, 1, fl);
        for (int k = 0; k < 784; k++) *mat_at(&ds->X,i,k) = pix[k]/255.0f;
        ds->labels[i] = lab;
        for (int c = 0; c < 10; c++) *mat_at(&ds->y,i,c) = (c==lab)?1.0f:0.0f;
    }
    free(pix); fclose(fi); fclose(fl);
    return ds;
}
