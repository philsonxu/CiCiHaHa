/**
 * @file ml_models.h
 * @brief 经典机器学习模型
 */
#ifndef ML_MODELS_H
#define ML_MODELS_H

#include "aimath.h"
#include "dataset.h"
#include <stddef.h>

/* ========== K近邻 KNN ========== */
typedef struct {
    int k;
    const Dataset* train;
} KNN;

KNN* knn_create(int k);
void knn_train(KNN* model, const Dataset* ds);
int knn_predict_one(const KNN* model, const float* x);
void knn_predict(const KNN* model, const Dataset* test, int* pred_labels);
void knn_free(KNN* model);

/* ========== 线性回归 (最小二乘闭式解 + 梯度下降) ========== */
typedef struct {
    size_t feat_dim;
    float* w;       /* feat_dim */
    float b;
    float lr;
    int epochs;
} LinearRegression;

LinearRegression* lr_create(size_t feat_dim, float lr, int epochs);
/* 梯度下降训练 */
void lr_train_gd(LinearRegression* model, const Dataset* ds);
/* 闭式解 (X^T X)^{-1} X^T y */
void lr_train_closed(LinearRegression* model, const Dataset* ds);
float lr_predict_one(const LinearRegression* model, const float* x);
void lr_predict(const LinearRegression* model, const Dataset* test, float* pred);
void lr_free(LinearRegression* model);

/* ========== 逻辑回归 (二分类, SGD) ========== */
typedef struct {
    size_t feat_dim;
    float* w;
    float b;
    float lr;
    float l2_reg;
    int epochs;
} LogisticRegression;

LogisticRegression* logreg_create(size_t feat_dim, float lr, float l2_reg, int epochs);
void logreg_train(LogisticRegression* model, const Dataset* ds);
float logreg_predict_proba(const LogisticRegression* model, const float* x);
int logreg_predict_one(const LogisticRegression* model, const float* x);
void logreg_predict(const LogisticRegression* model, const Dataset* test, int* pred);
void logreg_free(LogisticRegression* model);

/* ========== 多分类逻辑回归 (Softmax Regression, SGD) ========== */
typedef struct {
    size_t feat_dim;
    size_t num_classes;
    Matrix W;       /* feat_dim x num_classes */
    float* b;       /* num_classes */
    float lr;
    float l2_reg;
    int epochs;
} SoftmaxRegression;

SoftmaxRegression* sm_create(size_t feat_dim, size_t num_classes, float lr, float l2_reg, int epochs);
void sm_train(SoftmaxRegression* model, const Dataset* ds);
void sm_predict_proba(const SoftmaxRegression* model, const float* x, float* proba);
int sm_predict_one(const SoftmaxRegression* model, const float* x);
void sm_predict(const SoftmaxRegression* model, const Dataset* test, int* pred);
void sm_free(SoftmaxRegression* model);

/* ========== K-Means聚类 ========== */
typedef struct {
    size_t k;
    size_t feat_dim;
    Matrix centroids;   /* k x feat_dim */
    int* assignments;   /* n */
    int max_iters;
    float tol;
} KMeans;

KMeans* kmeans_create(size_t k, size_t feat_dim, int max_iters, float tol);
/* 返回迭代次数, inertia写入*inertia (簇内平方和) */
int kmeans_fit(KMeans* model, const Dataset* ds, float* inertia);
int kmeans_predict_one(const KMeans* model, const float* x);
void kmeans_free(KMeans* model);

/* ========== 决策树分类 (CART, 基尼指数) ========== */
#define DT_MAX_DEPTH 20
#define DT_MIN_SAMPLES 2

typedef struct DTNode {
    int is_leaf;
    int label;              /* 叶子: 预测类别 */
    size_t feat_idx;        /* 分裂特征 */
    float threshold;        /* 分裂阈值 */
    float gini;
    size_t n_samples;
    struct DTNode* left;    /* <= threshold */
    struct DTNode* right;   /* > threshold */
} DTNode;

typedef struct {
    DTNode* root;
    size_t max_depth;
    size_t min_samples_split;
    size_t feat_dim;
    size_t num_classes;
} DecisionTree;

DecisionTree* dt_create(size_t max_depth, size_t min_samples_split);
void dt_train(DecisionTree* model, const Dataset* ds);
int dt_predict_one(const DecisionTree* model, const float* x);
void dt_predict(const DecisionTree* model, const Dataset* test, int* pred);
void dt_free(DecisionTree* model);
/* 打印树结构 */
void dt_print(const DecisionTree* model);

/* ========== 高斯朴素贝叶斯 ========== */
typedef struct {
    size_t feat_dim;
    size_t num_classes;
    float* class_prior;         /* num_classes */
    float* theta;               /* num_classes x feat_dim, 均值 */
    float* sigma;               /* num_classes x feat_dim, 方差 */
    /* var_smoothing 加到sigma避免除零 */
    float var_smoothing;
} GaussianNB;

GaussianNB* gnb_create(size_t feat_dim, size_t num_classes, float var_smoothing);
void gnb_train(GaussianNB* model, const Dataset* ds);
int gnb_predict_one(const GaussianNB* model, const float* x);
void gnb_predict(const GaussianNB* model, const Dataset* test, int* pred);
void gnb_free(GaussianNB* model);

/* ========== PCA主成分分析 ========== */
typedef struct {
    size_t n_components;
    size_t feat_dim;
    Matrix components;      /* n_components x feat_dim */
    float* mean;            /* feat_dim */
    float* explained_var;   /* n_components, 特征值 */
} PCA;

PCA* pca_create(size_t n_components);
/* 使用幂法+deflation求解特征向量 */
void pca_fit(PCA* model, const Dataset* ds);
void pca_transform(const PCA* model, const Matrix* X, Matrix* X_proj);
void pca_free(PCA* model);

#endif
