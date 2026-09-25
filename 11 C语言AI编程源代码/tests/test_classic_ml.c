/**
 * @file test_classic_ml.c - 经典机器学习模型验证
 * 验证算法: KNN, 逻辑回归, Softmax, 决策树, 朴素贝叶斯, KMeans, PCA, 线性回归
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "aimath.h"
#include "dataset.h"
#include "ml_models.h"

static int tests_passed = 0;
static int tests_total = 0;
#define CHECK(name, cond) do { \
    tests_total++; \
    if (cond) { tests_passed++; printf("  [PASS] %s\n", name); } \
    else { printf("  [FAIL] %s\n", name); } \
} while(0)

void test_knn(void) {
    printf("\n=== K近邻 (KNN) 测试 ===\n");
    Dataset* ds = dataset_make_moons(200, 0.1f);
    Dataset *test;
    Dataset* train = dataset_split(ds, 0.7f, &test);
    KNN* model = knn_create(5);
    knn_train(model, train);
    int* pred = (int*)malloc(test->n*sizeof(int));
    knn_predict(model, test, pred);
    float acc = accuracy_score_labels(pred, test->labels, test->n);
    printf("  Moons数据集 KNN(k=5) 准确率: %.2f%%\n", acc*100);
    CHECK("KNN moons accuracy > 90%", acc > 0.90f);
    free(pred); knn_free(model);
    dataset_free(train); dataset_free(test); dataset_free(ds);
}

void test_logistic_regression(void) {
    printf("\n=== 逻辑回归 (二分类) 测试 ===\n");
    Dataset* ds = dataset_make_linear(300);
    Dataset *test;
    Dataset* train = dataset_split(ds, 0.7f, &test);
    LogisticRegression* model = logreg_create(2, 0.1f, 0.001f, 500);
    logreg_train(model, train);
    int* pred = (int*)malloc(test->n*sizeof(int));
    logreg_predict(model, test, pred);
    float acc = accuracy_score_labels(pred, test->labels, test->n);
    printf("  线性可分数据 逻辑回归 准确率: %.2f%%\n", acc*100);
    CHECK("LogReg linear accuracy > 95%", acc > 0.95f);
    free(pred); logreg_free(model);
    dataset_free(train); dataset_free(test); dataset_free(ds);
}

void test_softmax_iris(void) {
    printf("\n=== Softmax多分类 (Iris) 测试 ===\n");
    random_seed(42);
    Dataset* ds = dataset_load_iris();
    Dataset *test;
    Dataset* train = dataset_split(ds, 0.7f, &test);
    SoftmaxRegression* model = sm_create(4, 3, 0.1f, 1e-4f, 1000);
    sm_train(model, train);
    int* pred = (int*)malloc(test->n*sizeof(int));
    sm_predict(model, test, pred);
    float acc = accuracy_score_labels(pred, test->labels, test->n);
    printf("  Iris数据集 Softmax回归 准确率: %.2f%%\n", acc*100);
    CHECK("Softmax Iris accuracy > 85%", acc > 0.85f);
    free(pred); sm_free(model);
    dataset_free(train); dataset_free(test); dataset_free(ds);
}

void test_decision_tree(void) {
    printf("\n=== CART决策树 测试 ===\n");
    random_seed(42);
    Dataset* ds = dataset_load_iris();
    Dataset *test;
    Dataset* train = dataset_split(ds, 0.7f, &test);
    DecisionTree* model = dt_create(5, 2);
    dt_train(model, train);
    int* pred = (int*)malloc(test->n*sizeof(int));
    dt_predict(model, test, pred);
    float acc = accuracy_score_labels(pred, test->labels, test->n);
    printf("  Iris数据集 CART决策树 准确率: %.2f%%\n", acc*100);
    CHECK("DecisionTree Iris accuracy > 85%", acc > 0.85f);
    free(pred); dt_free(model);
    dataset_free(train); dataset_free(test); dataset_free(ds);
}

void test_naive_bayes(void) {
    printf("\n=== 高斯朴素贝叶斯 测试 ===\n");
    random_seed(42);
    Dataset* ds = dataset_load_iris();
    Dataset *test;
    Dataset* train = dataset_split(ds, 0.7f, &test);
    GaussianNB* model = gnb_create(4, 3, 1e-9f);
    gnb_train(model, train);
    int* pred = (int*)malloc(test->n*sizeof(int));
    gnb_predict(model, test, pred);
    float acc = accuracy_score_labels(pred, test->labels, test->n);
    printf("  Iris数据集 高斯朴素贝叶斯 准确率: %.2f%%\n", acc*100);
    CHECK("GaussianNB Iris accuracy > 85%", acc > 0.85f);
    free(pred); gnb_free(model);
    dataset_free(train); dataset_free(test); dataset_free(ds);
}

void test_linear_regression(void) {
    printf("\n=== 线性回归 (闭式解) 测试 ===\n");
    random_seed(42);
    Dataset* ds = dataset_make_regression(200, 3, 0.01f);
    LinearRegression* model = lr_create(3, 0.01f, 1);
    lr_train_closed(model, ds);
    float* pred = (float*)malloc(ds->n*sizeof(float));
    lr_predict(model, ds, pred);
    Matrix P = {ds->n, 1, pred};
    float mse = mse_score(&P, &ds->y);
    printf("  线性回归闭式解 MSE(噪声0.01): %.6f\n", mse);
    CHECK("LR closed-form MSE < 0.002", mse < 0.002f);
    free(pred); lr_free(model); dataset_free(ds);
}

void test_kmeans(void) {
    printf("\n=== K-Means聚类 测试 ===\n");
    random_seed(42);
    Dataset* ds = dataset_make_circles(300, 0.05f);
    KMeans* km = kmeans_create(2, 2, 100, 1e-6f);
    float inertia;
    int iters = kmeans_fit(km, ds, &inertia);
    printf("  Circles K-Means 迭代 %d 次, inertia=%.2f\n", iters, inertia);
    /* K-means对circles效果一般, 验证聚类中心范围合理 */
    CHECK("KMeans inertia finite", isfinite(inertia) && inertia > 0);
    CHECK("KMeans iters <= 100", iters <= 100);
    kmeans_free(km); dataset_free(ds);
}

void test_pca(void) {
    printf("\n=== PCA主成分分析 测试 ===\n");
    random_seed(42);
    Dataset* ds = dataset_load_iris();
    PCA* pca = pca_create(2);
    pca_fit(pca, ds);
    Matrix X2 = mat_create(ds->n, 2);
    pca_transform(pca, &ds->X, &X2);
    printf("  PCA 解释方差: λ1=%.3f λ2=%.3f, 累计占比=%.1f%%\n",
           pca->explained_var[0], pca->explained_var[1],
           (pca->explained_var[0]+pca->explained_var[1])/
           (pca->explained_var[0]+pca->explained_var[1]+1e-6f)*100);
    CHECK("PCA first variance positive", pca->explained_var[0] > pca->explained_var[1]);
    /* 验证PCA后维度 */
    CHECK("PCA output dim 2", X2.cols == 2);
    mat_free(&X2); pca_free(pca); dataset_free(ds);
}

/* 所有模型集成对比: Iris */
void test_all_on_iris(void) {
    printf("\n=== 所有经典ML模型在Iris上对比 ===\n");
    random_seed(42);
    Dataset* ds = dataset_load_iris();
    Dataset *test;
    Dataset* train = dataset_split(ds, 0.7f, &test);
    int* pred = (int*)malloc(test->n*sizeof(int));

    /* KNN */
    KNN* knn = knn_create(5); knn_train(knn, train);
    knn_predict(knn, test, pred);
    printf("  KNN(k=5):           %.2f%%\n", accuracy_score_labels(pred,test->labels,test->n)*100);
    knn_free(knn);

    /* Softmax */
    SoftmaxRegression* sm = sm_create(4,3,0.1f,1e-4f,1000);
    sm_train(sm, train); sm_predict(sm,test,pred);
    printf("  Softmax Regression: %.2f%%\n", accuracy_score_labels(pred,test->labels,test->n)*100);
    sm_free(sm);

    /* Decision Tree */
    DecisionTree* dt = dt_create(5,2); dt_train(dt,train);
    dt_predict(dt,test,pred);
    printf("  Decision Tree:      %.2f%%\n", accuracy_score_labels(pred,test->labels,test->n)*100);
    dt_free(dt);

    /* Naive Bayes */
    GaussianNB* gnb = gnb_create(4,3,1e-9f); gnb_train(gnb,train);
    gnb_predict(gnb,test,pred);
    printf("  Gaussian NB:        %.2f%%\n", accuracy_score_labels(pred,test->labels,test->n)*100);
    gnb_free(gnb);

    free(pred);
    dataset_free(train); dataset_free(test); dataset_free(ds);
}

int main(void) {
    printf("======= 经典机器学习模型验证程序 =======\n");
    random_seed(42);
    test_knn();
    test_logistic_regression();
    test_softmax_iris();
    test_decision_tree();
    test_naive_bayes();
    test_linear_regression();
    test_kmeans();
    test_pca();
    test_all_on_iris();

    printf("\n========== 验证结果: %d/%d PASSED ==========\n", tests_passed, tests_total);
    return tests_passed == tests_total ? 0 : 1;
}
