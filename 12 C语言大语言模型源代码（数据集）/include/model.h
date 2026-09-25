#ifndef CLLM_MODEL_H
#define CLLM_MODEL_H

#include "cllm.h"

// ========== Transformer 模型配置 ==========
typedef struct {
    int vocab_size;
    int d_model;       // C
    int n_heads;
    int n_layers;
    int block_size;    // T
    float lr;
    float beta1, beta2, eps, weight_decay;
    int batch_size;
} GPTConfig;

void gpt_config_default(GPTConfig *cfg);

// 参数 (采用分离式存储，便于AdamW)
typedef struct {
    // token embedding [V,C], pos embedding [T,C]
    float *wte, *wpe;
    // per layer:
    // ln1: gamma[C], beta[C]
    // Q/K/V/O 权重 [C,C], bias [C]
    // ln2: gamma[C], beta[C]
    // fc: [4C,C], bias [4C]
    // proj: [C,4C], bias [C]
    // 所有层参数连续存放:
    float *ln1g, *ln1b;  // [L*C]
    float *wq, *wk, *wv, *wo; // [L*C*C]
    float *bq, *bk, *bv, *bo; // [L*C]
    float *ln2g, *ln2b;  // [L*C]
    float *wfc;          // [L*4C*C]
    float *bfc;          // [L*4C]
    float *wproj;        // [L*C*4C]
    float *bproj;        // [L*C]
    float *lnfg, *lnfb;  // [C]
    // 梯度
    float *dwte, *dwpe;
    float *dln1g, *dln1b;
    float *dwq, *dwk, *dwv, *dwo;
    float *dbq, *dbk, *dbv, *dbo;
    float *dln2g, *dln2b;
    float *dwfc, *dbfc, *dwproj, *dbproj;
    float *dlnfg, *dlnfb;
    // AdamW m/v
    float *mwte, *vwte, *mwpe, *vwpe;
    float *mln1g, *vln1g, *mln1b, *vln1b;
    float *mwq, *vwq, *mwk, *vwk, *mwv, *vwv, *mwo, *vwo;
    float *mbq, *vbq, *mbk, *vbk, *mbv, *vbv, *mbo, *vbo;
    float *mln2g, *vln2g, *mln2b, *vln2b;
    float *mwfc, *vwfc, *mbfc, *vbfc, *mwproj, *vwproj, *mbproj, *vbproj;
    float *mlnfg, *vlnfg, *mlnfb, *vlnfb;
} GPTParams;

typedef struct GPTModel GPTModel;

GPTModel* gpt_create(const GPTConfig *cfg);
void      gpt_free(GPTModel *m);
float     gpt_forward_backward(GPTModel *m, const int *inputs, const int *targets);
void      gpt_step(GPTModel *m);
void      gpt_generate(GPTModel *m, const int *prompt, int plen, int *out, int n_new, float temp);
int       gpt_save(GPTModel *m, const char *path);
GPTModel* gpt_load(const char *path);
long long gpt_param_count(const GPTModel *m);
GPTConfig gpt_config(const GPTModel *m);

#endif
