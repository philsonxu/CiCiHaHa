/**
 * @file decision_tree.c - CART分类树 (基尼指数, 递归建树)
 */
#include "ml_models.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

static DTNode* _node_new(void) {
    DTNode* n = (DTNode*)calloc(1,sizeof(DTNode));
    return n;
}

static void _node_free(DTNode* n) {
    if (!n) return;
    _node_free(n->left); _node_free(n->right); free(n);
}

/* 计算样本集基尼指数 */
static float _gini(const int* labels, const size_t* idx, size_t n, size_t nc) {
    size_t cnt[16] = {0};
    for (size_t i=0;i<n;i++) cnt[labels[idx[i]]]++;
    float s=0; for(size_t c=0;c<nc;c++){float p=(float)cnt[c]/n;s+=p*p;}
    return 1.0f - s;
}

static DTNode* _build(
    const Dataset* ds,
    size_t* idx, size_t n,
    size_t depth, size_t max_depth, size_t min_split
) {
    DTNode* node = _node_new();
    node->n_samples = n;
    /* 统计众数作为默认标签 */
    size_t cnt[16] = {0}; size_t mc=0; int ml=0;
    for (size_t i=0;i<n;i++){int l=ds->labels[idx[i]];cnt[l]++;if(cnt[l]>mc){mc=cnt[l];ml=l;}}
    node->label = ml;
    node->gini = _gini(ds->labels, idx, n, ds->num_classes);

    /* 终止条件 */
    if (depth >= max_depth || n < min_split || node->gini < 1e-6f) {
        node->is_leaf = 1;
        return node;
    }

    /* 找最优分裂: 遍历特征, 对每个特征取样本分位数候选阈值 */
    float best_gain = 0;
    size_t best_feat = 0; float best_thr = 0; size_t best_left = 0;
    float total_gini = node->gini;

    for (size_t f=0; f<ds->feat_dim; f++) {
        /* 抽取候选阈值: 排序后取若干分位点 */
        float vals[64]; size_t vn = n<64?n:64;
        for (size_t i=0;i<vn;i++) vals[i] = *mat_at_c(&ds->X, idx[(i*n)/vn], f);
        /* 简单选择每个相邻对中点 */
        for (size_t v=0; v<vn; v++) {
            float thr = vals[v];
            size_t nl = 0;
            for (size_t i=0;i<n;i++) if (*mat_at_c(&ds->X, idx[i], f) <= thr) nl++;
            if (nl==0 || nl==n) continue;
            /* 计算左右基尼 */
            size_t* left = (size_t*)malloc(nl*sizeof(size_t));
            size_t* right = (size_t*)malloc((n-nl)*sizeof(size_t));
            size_t li=0,ri=0;
            for (size_t i=0;i<n;i++){
                if (*mat_at_c(&ds->X, idx[i], f) <= thr) left[li++]=idx[i];
                else right[ri++]=idx[i];
            }
            float g_l = _gini(ds->labels, left, nl, ds->num_classes);
            float g_r = _gini(ds->labels, right, n-nl, ds->num_classes);
            float gini_split = (float)nl/n*g_l + (float)(n-nl)/n*g_r;
            float gain = total_gini - gini_split;
            if (gain > best_gain) {
                best_gain = gain; best_feat = f; best_thr = thr; best_left = nl;
            }
            free(left); free(right);
        }
    }

    if (best_gain < 1e-9f) { node->is_leaf = 1; return node; }

    node->feat_idx = best_feat; node->threshold = best_thr;
    size_t* left_idx = (size_t*)malloc(best_left*sizeof(size_t));
    size_t* right_idx = (size_t*)malloc((n-best_left)*sizeof(size_t));
    size_t li=0,ri=0;
    for (size_t i=0;i<n;i++){
        if (*mat_at_c(&ds->X, idx[i], best_feat) <= best_thr) left_idx[li++]=idx[i];
        else right_idx[ri++]=idx[i];
    }
    node->left = _build(ds, left_idx, best_left, depth+1, max_depth, min_split);
    node->right = _build(ds, right_idx, n-best_left, depth+1, max_depth, min_split);
    free(left_idx); free(right_idx);
    return node;
}

DecisionTree* dt_create(size_t max_depth, size_t min_samples_split) {
    DecisionTree* m = (DecisionTree*)calloc(1,sizeof(DecisionTree));
    m->max_depth = max_depth ? max_depth : DT_MAX_DEPTH;
    m->min_samples_split = min_samples_split ? min_samples_split : DT_MIN_SAMPLES;
    m->root = NULL;
    return m;
}

void dt_train(DecisionTree* m, const Dataset* ds) {
    m->feat_dim = ds->feat_dim;
    m->num_classes = ds->num_classes;
    size_t* idx = (size_t*)malloc(ds->n*sizeof(size_t));
    for (size_t i=0;i<ds->n;i++) idx[i] = i;
    m->root = _build(ds, idx, ds->n, 0, m->max_depth, m->min_samples_split);
    free(idx);
}

static int _predict_node(const DTNode* n, const float* x) {
    if (n->is_leaf) return n->label;
    if (x[n->feat_idx] <= n->threshold) return _predict_node(n->left, x);
    else return _predict_node(n->right, x);
}
int dt_predict_one(const DecisionTree* m, const float* x) { return _predict_node(m->root, x); }
void dt_predict(const DecisionTree* m, const Dataset* t, int* pred) {
    for(size_t i=0;i<t->n;i++) pred[i] = dt_predict_one(m,(const float*)mat_at_c(&t->X,i,0));
}

static void _print_node(const DTNode* n, int depth) {
    for (int i=0;i<depth;i++) printf("  ");
    if (n->is_leaf) printf("Leaf: class=%d n=%zu gini=%.3f\n", n->label, n->n_samples, n->gini);
    else {
        printf("Node: feat[%zu] <= %.3f n=%zu gini=%.3f\n", n->feat_idx, n->threshold, n->n_samples, n->gini);
        _print_node(n->left, depth+1);
        _print_node(n->right, depth+1);
    }
}
void dt_print(const DecisionTree* m) { _print_node(m->root, 0); }
void dt_free(DecisionTree* m) { if(m){_node_free(m->root);free(m);} }
