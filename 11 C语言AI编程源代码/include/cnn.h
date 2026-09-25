/**
 * @file cnn.h
 * @brief 极简卷积神经网络 - Conv2d + ReLU + MaxPool + FC
 *
 * 支持LeNet-5风格的CNN, 专为MNIST手写数字识别优化
 * 纯C实现im2col + GEMM卷积
 */
#ifndef CNN_H
#define CNN_H

#include "aimath.h"
#include "dataset.h"
#include <stddef.h>

/* ========== 卷积层 ========== */
typedef struct {
    int in_c, out_c;        /* 输入/输出通道 */
    int kernel_h, kernel_w;
    int pad, stride;
    /* 权重 out_c x in_c x kh x kw, 内存按 out_c*(in_c*kh*kw) 线性存储 */
    float* W;
    float* b;               /* out_c */
    float* dW;
    float* db;
    /* 缓存 */
    float* col;             /* im2col缓存 */
    float* dcol;
    /* Adam状态 */
    float* VdW; float* Vdb;
    float* SdW; float* Sdb;
    /* 前向缓存输入 */
    int in_h, in_w;
    int out_h, out_w;
    float* input_cache;
} ConvLayer;

/* ========== MaxPool层 ========== */
typedef struct {
    int pool_h, pool_w;
    int stride;
    int in_c, in_h, in_w;
    int out_h, out_w;
    /* 缓存最大值位置 */
    int* mask;
} MaxPoolLayer;

/* ========== ReLU ========== */
typedef struct { float* mask; int n; } ReLULayer;

/* ========== 全连接层 ========== */
typedef struct {
    int in_dim, out_dim;
    Matrix W;
    float* b;
    float* dW; float* db;
    Matrix Z, A;
    Matrix A_prev_flat;
    /* Adam */
    Matrix VdW, SdW;
    float* Vdb; float* Sdb;
} FCLayer;

/* ========== Softmax+CE损失 ========== */

/* ========== 完整CNN (LeNet式) ========== */
typedef struct {
    /* 输入固定: 28x28 单通道 (MNIST) */
    /* Conv1: 1->8, 3x3, pad=1 -> 28x28x8 */
    ConvLayer conv1;
    ReLULayer relu1;
    /* Pool: 2x2 -> 14x14x8 */
    MaxPoolLayer pool1;
    /* Conv2: 8->16, 3x3, pad=1 -> 14x14x16 */
    ConvLayer conv2;
    ReLULayer relu2;
    /* Pool: 2x2 -> 7x7x16=784 */
    MaxPoolLayer pool2;
    /* FC1: 784->128 */
    FCLayer fc1;
    ReLULayer relu3;
    /* Dropout */
    float dropout_rate;
    float* dropout_mask;
    /* FC2: 128->10 */
    FCLayer fc2;
    /* softmax输出 */
    float* probs;   /* 10 */

    /* 优化器参数 */
    float lr;
    float beta1, beta2, eps;
    float l2_reg;
    size_t t;
    int verbose;

    /* 临时缓存 */
    int batch_size;
    float* conv1_out;  /* 每层输出缓存，形状见各层 */
    float* pool1_out;
    float* conv2_out;
    float* pool2_out;
    float* pool2_flat;
    float* fc1_out;
} CNN;

CNN* cnn_create(void);
void cnn_free(CNN* net);
void cnn_set_adam(CNN* net, float lr, float beta1, float beta2, float eps, float l2);
void cnn_set_dropout(CNN* net, float rate);
void cnn_set_verbose(CNN* net, int v);

/* 训练单张 (batch=1 SGD简化版), 返回loss */
float cnn_train_one(CNN* net, const float* img /*28x28*/, int label);

/* 预测单张, 返回0-9类别 */
int cnn_predict_one(CNN* net, const float* img);
/* 评估, 返回准确率 */
float cnn_evaluate(CNN* net, const Dataset* ds, size_t max_n);

/* 完整训练 */
void cnn_fit(CNN* net, const Dataset* train, const Dataset* test, int epochs);

int cnn_save(const CNN* net, const char* path);
CNN* cnn_load(const char* path);

#endif
