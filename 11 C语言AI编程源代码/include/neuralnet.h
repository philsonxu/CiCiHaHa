/**
 * @file neuralnet.h
 * @brief 前馈神经网络 (全连接MLP) - 从零实现
 *
 * 支持:
 *   - 任意层数、每层任意神经元数
 *   - 激活函数: sigmoid / tanh / relu / leaky_relu / softmax
 *   - 损失: MSE (回归) / CrossEntropy+Softmax (分类, 数值稳定版本)
 *   - 优化器: SGD / SGD+Momentum / Adam
 *   - 正则化: L2权重衰减 / Dropout
 *   - Mini-batch训练
 */
#ifndef NEURALNET_H
#define NEURALNET_H

#include "aimath.h"
#include "dataset.h"
#include <stddef.h>

typedef enum {
    ACT_SIGMOID = 0,
    ACT_TANH    = 1,
    ACT_RELU    = 2,
    ACT_LEAKY_RELU = 3,
    ACT_LINEAR  = 4,
    ACT_SOFTMAX = 5,
} Activation;

typedef enum {
    LOSS_MSE = 0,
    LOSS_CROSSENTROPY = 1,
} LossType;

typedef enum {
    OPT_SGD = 0,
    OPT_SGD_MOMENTUM = 1,
    OPT_ADAM = 2,
} OptimizerType;

/* 单层网络 */
typedef struct {
    size_t in_dim, out_dim;
    Activation act;

    Matrix W;       /* in_dim x out_dim */
    float* b;       /* out_dim */

    /* 前向缓存 */
    Matrix Z;       /* batch x out_dim, 线性输出 Wx+b */
    Matrix A;       /* batch x out_dim, 激活后 */
    Matrix A_prev;  /* batch x in_dim, 上一层输入 (缓存) */

    /* 梯度 */
    Matrix dW;
    float* db;

    /* 优化器状态 */
    Matrix VdW;     /* Momentum / Adam: 一阶矩 */
    float* Vdb;
    Matrix SdW;     /* Adam: 二阶矩 */
    float* Sdb;
} Layer;

typedef struct {
    size_t n_layers;        /* 隐藏层+输出层 总数 */
    Layer* layers;          /* [0]=第一隐藏层, [n_layers-1]=输出层 */

    LossType loss;
    OptimizerType opt;
    float lr;
    float momentum;         /* 动量系数 beta1 */
    float beta2;            /* Adam beta2 */
    float eps;              /* Adam epsilon */
    float l2_reg;           /* L2权重衰减 */
    float dropout_rate;     /* Dropout概率, 0表示不使用 */

    size_t batch_size;
    int verbose;

    /* 训练过程记录 */
    float* train_loss_history;
    float* train_acc_history;
    float* val_loss_history;
    float* val_acc_history;
    size_t history_len;
    size_t history_cap;

    /* Adam步长 */
    size_t t;
} NeuralNet;

/**
 * 创建神经网络
 * @param layer_dims 数组: [input_dim, hidden1, hidden2, ..., output_dim]
 * @param acts       数组: 每层激活函数 (长度 = n_layers = layer_dims长度-1)
 * @param n_layer_dims layer_dims 数组长度
 */
NeuralNet* nn_create(const size_t* layer_dims, const Activation* acts, size_t n_layer_dims);
void nn_free(NeuralNet* net);

void nn_set_loss(NeuralNet* net, LossType loss);
void nn_set_optimizer_sgd(NeuralNet* net, float lr, float l2_reg);
void nn_set_optimizer_momentum(NeuralNet* net, float lr, float momentum, float l2_reg);
void nn_set_optimizer_adam(NeuralNet* net, float lr, float beta1, float beta2, float eps, float l2_reg);
void nn_set_batch_size(NeuralNet* net, size_t bs);
void nn_set_dropout(NeuralNet* net, float rate);
void nn_set_verbose(NeuralNet* net, int v);

/* 前向传播, 输入 X(batch x in), 输出返回最后一层A */
const Matrix* nn_forward(NeuralNet* net, const Matrix* X, int training);
/* 反向传播: 给定真值y(batch x out), 计算各层梯度 */
void nn_backward(NeuralNet* net, const Matrix* y);
/* 按当前优化器更新参数 */
void nn_update_params(NeuralNet* net);

/* 单epoch训练, 返回平均损失 */
float nn_train_epoch(NeuralNet* net, const Dataset* ds);
/* 完整训练 */
void nn_fit(NeuralNet* net, const Dataset* train, const Dataset* val, int epochs);

/* 预测 */
void nn_predict_proba(NeuralNet* net, const Matrix* X, Matrix* proba);
int nn_predict_one(NeuralNet* net, const float* x);
void nn_predict(NeuralNet* net, const Dataset* test, int* pred);
float nn_evaluate(NeuralNet* net, const Dataset* ds, float* loss_out, float* acc_out);

/* 保存/加载权重 */
int nn_save(const NeuralNet* net, const char* path);
NeuralNet* nn_load(const char* path);

#endif
