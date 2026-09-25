/**
 * @file dataset.h
 * @brief 数据集加载与处理工具
 */
#ifndef DATASET_H
#define DATASET_H

#include "aimath.h"
#include <stddef.h>

typedef struct {
    Matrix X;       /* N x feature_dim */
    Matrix y;       /* N x label_dim (one-hot) 或 N x 1 (回归值) */
    int* labels;    /* N, 离散标签 (0..num_classes-1) */
    size_t n;       /* 样本数量 */
    size_t feat_dim;
    size_t num_classes;
} Dataset;

Dataset* dataset_create(size_t n, size_t feat_dim, size_t num_classes);
void dataset_free(Dataset* ds);

/* 打乱数据集顺序 */
void dataset_shuffle(Dataset* ds);

/* 按比例拆分训练/测试集, train_ratio ∈ (0,1), 返回测试集 */
Dataset* dataset_split(Dataset* ds, float train_ratio, Dataset** test_ds);

/* 标准化: X=(X-mean)/std, 按列计算 */
void dataset_normalize(Dataset* ds, const float* mean, const float* std);
void dataset_compute_mean_std(const Dataset* ds, float* mean, float* std);

/* ========== Iris鸢尾花数据集（内置） ========== */
Dataset* dataset_load_iris(void);

/* ========== 生成数据集 ========== */
/* 线性可分两分类 */
Dataset* dataset_make_linear(size_t n);
/* 月牙形两分类 (moons) */
Dataset* dataset_make_moons(size_t n, float noise);
/* 同心圆两分类 (circles) */
Dataset* dataset_make_circles(size_t n, float noise);
/* XOR问题 (非线性) */
Dataset* dataset_make_xor(size_t n);
/* 线性回归数据 y = w·x + b + noise */
Dataset* dataset_make_regression(size_t n, size_t feat_dim, float noise);

/* ========== MNIST支持 ========== */
/* 加载二进制IDX格式MNIST, 像素归一化到[0,1] */
Dataset* dataset_load_mnist(const char* images_path, const char* labels_path, size_t max_n);

/* ========== 工具函数 ========== */
/* 计算准确率: argmax(pred) == argmax(y_true) */
float accuracy_score(const Matrix* y_pred, const Matrix* y_true);
float accuracy_score_labels(const int* pred, const int* true_label, size_t n);

/* MSE */
float mse_score(const Matrix* y_pred, const Matrix* y_true);

#endif
