#ifndef CLLM_DATASET_H
#define CLLM_DATASET_H

#include "cllm.h"

// 字符级文本数据集
typedef struct {
    char *text;          // 原始文本
    int  *tokens;        // 整数token
    long long length;    // 文本长度
    int  vocab_size;     // 词表大小
    char itos[256];      // index to char
    int  stoi[256];      // char to index
} CharDataset;

// 从文件加载文本数据集，构建字符词表
CharDataset* dataset_load(const char *path);
void         dataset_free(CharDataset *ds);

// 获取一个batch：inputs [B,T], targets [B,T]
// targets[t] = inputs[t+1]（自回归下一个token预测）
void dataset_get_batch(CharDataset *ds, int B, int T, int *inputs, int *targets);

// 内置莎士比亚tiny-shakespeare生成器：如果文件不存在，内置一个
// 足够训练的样本文本（几百到几千字符）保证离线可跑
void dataset_write_tiny_shakespeare(const char *path);

#endif
