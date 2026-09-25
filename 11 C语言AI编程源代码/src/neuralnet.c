/**
 * @file neuralnet.c - 全连接前馈神经网络MLP
 */
#include "neuralnet.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <assert.h>

static Layer _layer_create(size_t in, size_t out, Activation act) {
    Layer l; memset(&l, 0, sizeof(l));
    l.in_dim=in; l.out_dim=out; l.act=act;
    l.W = mat_create(in, out);
    l.b = (float*)calloc(out, sizeof(float));
    l.dW = mat_create(in, out);
    l.db = (float*)calloc(out, sizeof(float));
    l.VdW = mat_create(in, out); l.Vdb=(float*)calloc(out,sizeof(float));
    l.SdW = mat_create(in, out); l.Sdb=(float*)calloc(out,sizeof(float));
    return l;
}
static void _layer_free(Layer* l) {
    mat_free(&l->W); free(l->b);
    mat_free(&l->Z); mat_free(&l->A); mat_free(&l->A_prev);
    mat_free(&l->dW); free(l->db);
    mat_free(&l->VdW); free(l->Vdb);
    mat_free(&l->SdW); free(l->Sdb);
}

NeuralNet* nn_create(const size_t* layer_dims, const Activation* acts, size_t nld) {
    NeuralNet* net = (NeuralNet*)calloc(1,sizeof(NeuralNet));
    net->n_layers = nld-1;
    net->layers = (Layer*)calloc(net->n_layers, sizeof(Layer));
    for (size_t i=0;i<net->n_layers;i++) {
        net->layers[i] = _layer_create(layer_dims[i], layer_dims[i+1], acts[i]);
        /* 默认初始化: 隐藏层He, 输出层Xavier */
        if (acts[i] == ACT_RELU || acts[i] == ACT_LEAKY_RELU)
            init_he(&net->layers[i].W, layer_dims[i]);
        else
            init_xavier(&net->layers[i].W, layer_dims[i], layer_dims[i+1]);
    }
    net->loss = LOSS_CROSSENTROPY;
    net->opt = OPT_ADAM;
    net->lr = 0.001f;
    net->momentum = 0.9f;
    net->beta2 = 0.999f;
    net->eps = 1e-8f;
    net->l2_reg = 0;
    net->dropout_rate = 0;
    net->batch_size = 32;
    net->verbose = 1;
    net->history_cap = 256; net->history_len=0;
    net->train_loss_history=(float*)malloc(net->history_cap*sizeof(float));
    net->train_acc_history=(float*)malloc(net->history_cap*sizeof(float));
    net->val_loss_history=(float*)malloc(net->history_cap*sizeof(float));
    net->val_acc_history=(float*)malloc(net->history_cap*sizeof(float));
    net->t = 0;
    return net;
}
void nn_free(NeuralNet* net) {
    if (!net) return;
    for (size_t i=0;i<net->n_layers;i++) _layer_free(&net->layers[i]);
    free(net->layers);
    free(net->train_loss_history); free(net->train_acc_history);
    free(net->val_loss_history); free(net->val_acc_history);
    free(net);
}

void nn_set_loss(NeuralNet* net, LossType l) { net->loss = l; }
void nn_set_optimizer_sgd(NeuralNet* net, float lr, float l2) {
    net->opt=OPT_SGD; net->lr=lr; net->l2_reg=l2;
}
void nn_set_optimizer_momentum(NeuralNet* net, float lr, float mom, float l2) {
    net->opt=OPT_SGD_MOMENTUM; net->lr=lr; net->momentum=mom; net->l2_reg=l2;
}
void nn_set_optimizer_adam(NeuralNet* net, float lr, float b1, float b2, float eps, float l2) {
    net->opt=OPT_ADAM; net->lr=lr; net->momentum=b1; net->beta2=b2; net->eps=eps; net->l2_reg=l2;
}
void nn_set_batch_size(NeuralNet* net, size_t bs) { net->batch_size = bs; }
void nn_set_dropout(NeuralNet* net, float r) { net->dropout_rate = r; }
void nn_set_verbose(NeuralNet* net, int v) { net->verbose = v; }

/* ===== 激活函数应用 (element-wise) ===== */
static void _apply_act(Activation act, const Matrix* Z, Matrix* A) {
    size_t n = Z->rows*Z->cols;
    switch(act) {
        case ACT_SIGMOID: for(size_t i=0;i<n;i++) A->data[i]=sigmoid(Z->data[i]); break;
        case ACT_TANH: for(size_t i=0;i<n;i++) A->data[i]=tanh_act(Z->data[i]); break;
        case ACT_RELU: for(size_t i=0;i<n;i++) A->data[i]=relu(Z->data[i]); break;
        case ACT_LEAKY_RELU: for(size_t i=0;i<n;i++) A->data[i]=leaky_relu(Z->data[i]); break;
        case ACT_LINEAR: memcpy(A->data, Z->data, n*sizeof(float)); break;
        case ACT_SOFTMAX: {
            memcpy(A->data, Z->data, n*sizeof(float));
            for (size_t i=0;i<Z->rows;i++) softmax_row(mat_at(A,i,0), Z->cols);
            break;
        }
    }
}
/* 激活函数导数: dA/dZ 作用在Z (或A, 取决于激活), 输出逐元素导数 */
static float _act_deriv(Activation act, float z_or_a, float z_val) {
    (void)z_val;
    switch(act) {
        case ACT_SIGMOID: return sigmoid_deriv(z_or_a);
        case ACT_TANH: return tanh_deriv(z_or_a);
        case ACT_RELU: return relu_deriv(z_or_a);
        case ACT_LEAKY_RELU: return leaky_relu_deriv(z_or_a);
        case ACT_LINEAR: return 1.0f;
        default: return 1.0f;
    }
}

const Matrix* nn_forward(NeuralNet* net, const Matrix* X, int training) {
    size_t bs = X->rows;
    const Matrix* prev_A = X;
    (void)training;
    for (size_t l=0; l<net->n_layers; l++) {
        Layer* L = &net->layers[l];
        /* 确保缓存尺寸匹配 */
        if (L->Z.rows!=bs || L->Z.cols!=L->out_dim) {
            mat_free(&L->Z); mat_free(&L->A); mat_free(&L->A_prev);
            L->Z = mat_create(bs, L->out_dim);
            L->A = mat_create(bs, L->out_dim);
            L->A_prev = mat_create(bs, L->in_dim);
        }
        mat_copy(prev_A, &L->A_prev);
        /* Z = A_prev * W + b (broadcast) */
        mat_mul(&L->A_prev, &L->W, &L->Z);
        for (size_t i=0;i<bs;i++)
            for (size_t j=0;j<L->out_dim;j++)
                *mat_at(&L->Z,i,j) += L->b[j];
        /* 激活 */
        _apply_act(L->act, &L->Z, &L->A);
        prev_A = &L->A;
    }
    return &net->layers[net->n_layers-1].A;
}

void nn_backward(NeuralNet* net, const Matrix* y) {
    size_t bs = y->rows;
    /* 先算输出层 dA (对损失) */
    Layer* out = &net->layers[net->n_layers-1];
    Matrix dZ = mat_create(bs, out->out_dim);
    assert(out->Z.rows == bs && out->A.rows == bs);

    /* 损失对输出Z的梯度: dZ = A-y (不除bs, SGD单样本尺度) */
    if (net->loss == LOSS_CROSSENTROPY && out->act == ACT_SOFTMAX) {
        for (size_t i=0;i<bs*out->out_dim;i++) dZ.data[i] = out->A.data[i] - y->data[i];
    } else if (net->loss == LOSS_MSE && out->act == ACT_LINEAR) {
        for (size_t i=0;i<bs*out->out_dim;i++) dZ.data[i] = 2.0f*(out->A.data[i] - y->data[i]);
    } else {
        Matrix dA = mat_create(bs, out->out_dim);
        if (net->loss == LOSS_MSE) {
            for (size_t i=0;i<bs*out->out_dim;i++) dA.data[i] = 2.0f*(out->A.data[i]-y->data[i]);
        } else {
            for (size_t i=0;i<bs*out->out_dim;i++)
                dA.data[i] = -y->data[i]/(out->A.data[i]+1e-12f);
        }
        for (size_t i=0;i<bs*out->out_dim;i++)
            dZ.data[i] = dA.data[i] * _act_deriv(out->act, out->Z.data[i], out->A.data[i]);
        mat_free(&dA);
    }

    /* 反向传播 */
    Matrix dA_prev; dA_prev.rows=0; dA_prev.cols=0; dA_prev.data=NULL;
    for (int li = (int)net->n_layers-1; li >= 0; li--) {
        Layer* L = &net->layers[li];
        mat_fill(&L->dW, 0);
        mat_transpose_mul(&L->A_prev, &dZ, &L->dW);
        mat_scale(&L->dW, 1.0f/bs);
        vec_fill(L->db, 0, L->out_dim);
        for (size_t i=0;i<bs;i++)
            for (size_t j=0;j<L->out_dim;j++) L->db[j] += *mat_at_c(&dZ,i,j);
        for (size_t j=0;j<L->out_dim;j++) L->db[j] /= bs;
        /* dA_prev */
        if (li > 0) {
            Layer* prev = &net->layers[li-1];
            if (dA_prev.rows!=bs || dA_prev.cols!=L->in_dim) {
                mat_free(&dA_prev); dA_prev = mat_create(bs, L->in_dim);
            }
            mat_mul_transpose(&dZ, &L->W, &dA_prev);
            /* dZ for next layer = dA_prev .* act'(Z_prev) */
            for (size_t i=0;i<bs*prev->out_dim;i++)
            mat_free(&dZ);
            dZ = mat_create(bs, prev->out_dim);
            for (size_t i=0;i<bs;i++)
                for (size_t j=0;j<prev->out_dim;j++) {
                    float deriv;
                    switch(prev->act) {
                        case ACT_RELU:
                        case ACT_LEAKY_RELU:
                        case ACT_LINEAR:
                            deriv = _act_deriv(prev->act, prev->Z.data[i*prev->out_dim+j], 0);
                            break;
                        case ACT_SIGMOID:
                        case ACT_TANH:
                        default:
                            deriv = _act_deriv(prev->act, prev->A.data[i*prev->out_dim+j], 0);
                            break;
                    }
                    *mat_at(&dZ,i,j) = *mat_at_c(&dA_prev,i,j) * deriv;
                }
        }
        /* L2 */
        if (net->l2_reg > 0)
            for (size_t i=0;i<L->in_dim*L->out_dim;i++) L->dW.data[i] += net->l2_reg * L->W.data[i];
    }
    mat_free(&dZ);
    mat_free(&dA_prev);
}

void nn_update_params(NeuralNet* net) {
    net->t++;
    for (size_t l=0; l<net->n_layers; l++) {
        Layer* L = &net->layers[l];
        size_t nw = L->in_dim*L->out_dim;
        if (net->opt == OPT_SGD) {
            for (size_t i=0;i<nw;i++) L->W.data[i] -= net->lr * L->dW.data[i];
            for (size_t j=0;j<L->out_dim;j++) L->b[j] -= net->lr * L->db[j];
        } else if (net->opt == OPT_SGD_MOMENTUM) {
            float beta = net->momentum;
            for (size_t i=0;i<nw;i++) {
                L->VdW.data[i] = beta*L->VdW.data[i] + (1-beta)*L->dW.data[i];
                L->W.data[i] -= net->lr * L->VdW.data[i];
            }
            for (size_t j=0;j<L->out_dim;j++) {
                L->Vdb[j] = beta*L->Vdb[j] + (1-beta)*L->db[j];
                L->b[j] -= net->lr * L->Vdb[j];
            }
        } else if (net->opt == OPT_ADAM) {
            float b1=net->momentum, b2=net->beta2;
            for (size_t i=0;i<nw;i++) {
                L->VdW.data[i] = b1*L->VdW.data[i] + (1-b1)*L->dW.data[i];
                L->SdW.data[i] = b2*L->SdW.data[i] + (1-b2)*L->dW.data[i]*L->dW.data[i];
                float vc = L->VdW.data[i]/(1-powf(b1,(float)net->t));
                float sc = L->SdW.data[i]/(1-powf(b2,(float)net->t));
                L->W.data[i] -= net->lr * vc/(sqrtf(sc)+net->eps);
            }
            for (size_t j=0;j<L->out_dim;j++) {
                L->Vdb[j] = b1*L->Vdb[j] + (1-b1)*L->db[j];
                L->Sdb[j] = b2*L->Sdb[j] + (1-b2)*L->db[j]*L->db[j];
                float vc = L->Vdb[j]/(1-powf(b1,(float)net->t));
                float sc = L->Sdb[j]/(1-powf(b2,(float)net->t));
                L->b[j] -= net->lr * vc/(sqrtf(sc)+net->eps);
            }
        }
    }
}

float nn_train_epoch(NeuralNet* net, const Dataset* ds) {
    size_t n = ds->n;
    Dataset* shuffled = dataset_create(n, ds->feat_dim, ds->num_classes);
    mat_copy(&ds->X, &shuffled->X);
    mat_copy(&ds->y, &shuffled->y);
    if (ds->labels && shuffled->labels)
        memcpy(shuffled->labels, ds->labels, n*sizeof(int));
    shuffled->n = n;
    dataset_shuffle(shuffled);

    float total_loss = 0; size_t nb = 0;
    size_t bs = net->batch_size;
    if (bs > n) bs = n;
    Matrix Xb = mat_create(bs, ds->feat_dim);
    Matrix yb = mat_create(bs, ds->y.cols);

    for (size_t start = 0; start + bs <= n; start += bs) {
        for (size_t i=0;i<bs;i++){
            memcpy(mat_at(&Xb,i,0), mat_at_c(&shuffled->X,start+i,0), ds->feat_dim*sizeof(float));
            memcpy(mat_at(&yb,i,0), mat_at_c(&shuffled->y,start+i,0), ds->y.cols*sizeof(float));
        }
        const Matrix* out = nn_forward(net, &Xb, 1);
        float loss = 0;
        for (size_t i=0;i<bs;i++) {
            if (net->loss == LOSS_CROSSENTROPY) {
                for (size_t j=0;j<ds->y.cols;j++)
                    loss += -*mat_at_c(&yb,i,j)*logf(*mat_at_c(out,i,j)+1e-12f);
            } else {
                for (size_t j=0;j<ds->y.cols;j++){float d=*mat_at_c(out,i,j)-*mat_at_c(&yb,i,j); loss+=0.5f*d*d;}
            }
        }
        total_loss += loss;
        nn_backward(net, &yb);
        nn_update_params(net);
        nb++;
    }
    mat_free(&Xb); mat_free(&yb);
    dataset_free(shuffled);
    return total_loss/(nb*bs);
}

void nn_fit(NeuralNet* net, const Dataset* train, const Dataset* val, int epochs) {
    for (int ep=0;ep<epochs;ep++) {
        float tr_loss = nn_train_epoch(net, train);
        float tr_acc = 0, v_loss=0, v_acc=0;
        if (train->num_classes >= 2 || net->loss == LOSS_MSE) {
            nn_evaluate(net, train, NULL, &tr_acc);
            if (train->num_classes < 2) tr_acc = 0; /* regression无acc */
        }
        if (val) {
            nn_evaluate(net, val, &v_loss, &v_acc);
            if (val->num_classes < 2) v_acc = 0;
        }
        if (net->verbose && (ep%max(1,epochs/20)==0 || ep==epochs-1)) {
            printf("  [Epoch %3d/%d] train_loss=%.4f train_acc=%.2f%%", ep+1, epochs, tr_loss, tr_acc*100);
            if (val) printf(" val_loss=%.4f val_acc=%.2f%%", v_loss, v_acc*100);
            printf("\n");
        }
        /* 记录历史 */
        if (net->history_len >= net->history_cap) {
            net->history_cap *= 2;
            net->train_loss_history=(float*)realloc(net->train_loss_history, net->history_cap*sizeof(float));
            net->train_acc_history=(float*)realloc(net->train_acc_history, net->history_cap*sizeof(float));
            net->val_loss_history=(float*)realloc(net->val_loss_history, net->history_cap*sizeof(float));
            net->val_acc_history=(float*)realloc(net->val_acc_history, net->history_cap*sizeof(float));
        }
        net->train_loss_history[net->history_len]=tr_loss;
        net->train_acc_history[net->history_len]=tr_acc;
        net->val_loss_history[net->history_len]=v_loss;
        net->val_acc_history[net->history_len]=v_acc;
        net->history_len++;
    }
}

void nn_predict_proba(NeuralNet* net, const Matrix* X, Matrix* proba) {
    const Matrix* out = nn_forward(net, X, 0);
    mat_copy(out, proba);
}

int nn_predict_one(NeuralNet* net, const float* x) {
    Matrix X = mat_create(1, net->layers[0].in_dim);
    memcpy(X.data, x, net->layers[0].in_dim*sizeof(float));
    const Matrix* out = nn_forward(net, &X, 0);
    size_t mi; vec_max(out->data, out->cols, &mi);
    mat_free(&X);
    return (int)mi;
}

void nn_predict(NeuralNet* net, const Dataset* test, int* pred) {
    size_t bs = 128;
    for (size_t s=0;s<test->n;s+=bs) {
        size_t e = s+bs; if(e>test->n)e=test->n;
        size_t cnt = e-s;
        Matrix Xb = mat_create(bs, test->feat_dim);
        for (size_t i=0;i<cnt;i++) memcpy(mat_at(&Xb,i,0),mat_at_c(&test->X,s+i,0),test->feat_dim*sizeof(float));
        for (size_t i=cnt;i<bs;i++) memset(mat_at(&Xb,i,0),0,test->feat_dim*sizeof(float));
        const Matrix* out = nn_forward(net, &Xb, 0);
        for (size_t i=0;i<cnt;i++) {
            size_t mi; vec_max((const float*)mat_at_c(out,i,0), out->cols, &mi);
            pred[s+i]=(int)mi;
        }
        mat_free(&Xb);
    }
}

float nn_evaluate(NeuralNet* net, const Dataset* ds, float* loss_out, float* acc_out) {
    int* pred = (int*)malloc(ds->n*sizeof(int));
    nn_predict(net, ds, pred);
    float acc = accuracy_score_labels(pred, ds->labels, ds->n);
    /* 计算loss: 分批 */
    float total_loss = 0;
    size_t bs = 128;
    Matrix Xb = mat_create(bs, ds->feat_dim);
    Matrix yb = mat_create(bs, ds->y.cols);
    for (size_t s=0; s<ds->n; s+=bs) {
        size_t e = s+bs; if (e>ds->n) e=ds->n;
        size_t cnt = e-s;
        for (size_t i=0;i<cnt;i++){
            memcpy(mat_at(&Xb,i,0), mat_at_c(&ds->X,s+i,0), ds->feat_dim*sizeof(float));
            memcpy(mat_at(&yb,i,0), mat_at_c(&ds->y,s+i,0), ds->y.cols*sizeof(float));
        }
        for (size_t i=cnt;i<bs;i++){memset(mat_at(&Xb,i,0),0,ds->feat_dim*sizeof(float));memset(mat_at(&yb,i,0),0,ds->y.cols*sizeof(float));}
        const Matrix* out = nn_forward(net, &Xb, 0);
        for (size_t i=0;i<cnt;i++) {
            if (net->loss == LOSS_CROSSENTROPY)
                for (size_t j=0;j<ds->y.cols;j++) total_loss += -*mat_at_c(&yb,i,j)*logf(*mat_at_c(out,i,j)+1e-12f);
            else
                for (size_t j=0;j<ds->y.cols;j++){float d=*mat_at_c(out,i,j)-*mat_at_c(&yb,i,j); total_loss+=0.5f*d*d;}
        }
        if (cnt<bs) break;
    }
    mat_free(&Xb); mat_free(&yb);
    float loss = total_loss/ds->n;
    if (loss_out)*loss_out=loss;
    if (acc_out)*acc_out=acc;
    free(pred);
    return acc;
}

int nn_save(const NeuralNet* net, const char* path) {
    FILE* f = fopen(path, "wb");
    if (!f) return -1;
    /* 二进制头: magic + n_layers + 每层(in,out,act) 全用uint32 */
    const char magic[] = "CAINN1";
    fwrite(magic, 1, 6, f);
    unsigned int nl = (unsigned int)net->n_layers;
    fwrite(&nl, sizeof(unsigned int), 1, f);
    for (size_t l=0;l<net->n_layers;l++){
        const Layer* L=&net->layers[l];
        unsigned int in=(unsigned int)L->in_dim, out=(unsigned int)L->out_dim, act=(unsigned int)L->act;
        fwrite(&in,sizeof(unsigned int),1,f);
        fwrite(&out,sizeof(unsigned int),1,f);
        fwrite(&act,sizeof(unsigned int),1,f);
    }
    for (size_t l=0;l<net->n_layers;l++){
        const Layer* L=&net->layers[l];
        fwrite(L->W.data, sizeof(float), L->in_dim*L->out_dim, f);
        fwrite(L->b, sizeof(float), L->out_dim, f);
    }
    fclose(f); return 0;
}

NeuralNet* nn_load(const char* path) {
    FILE* f = fopen(path, "rb");
    if (!f) return NULL;
    char magic[6];
    if (fread(magic,1,6,f)!=6) {fclose(f);return NULL;}
    if (memcmp(magic,"CAINN1",6)!=0){fclose(f);return NULL;}
    unsigned int nl;
    if (fread(&nl,sizeof(unsigned int),1,f)!=1){fclose(f);return NULL;}
    size_t n_layers = nl;
    size_t* in_dim=(size_t*)malloc(n_layers*sizeof(size_t));
    size_t* out_dim=(size_t*)malloc(n_layers*sizeof(size_t));
    Activation* acts=(Activation*)malloc(n_layers*sizeof(Activation));
    for (size_t l=0;l<n_layers;l++){
        unsigned int in,out,a;
        if (fread(&in,sizeof(unsigned int),1,f)!=1)break;
        if (fread(&out,sizeof(unsigned int),1,f)!=1)break;
        if (fread(&a,sizeof(unsigned int),1,f)!=1)break;
        in_dim[l]=in;out_dim[l]=out;acts[l]=(Activation)a;
    }
    size_t* dims=(size_t*)malloc((n_layers+1)*sizeof(size_t));
    dims[0]=in_dim[0];
    for(size_t l=0;l<n_layers;l++) dims[l+1]=out_dim[l];
    NeuralNet* net = nn_create(dims, acts, n_layers+1);
    for (size_t l=0;l<n_layers;l++){
        Layer* L=&net->layers[l];
        fread(L->W.data, sizeof(float), in_dim[l]*out_dim[l], f);
        fread(L->b, sizeof(float), out_dim[l], f);
    }
    fclose(f);
    free(in_dim);free(out_dim);free(acts);free(dims);
    return net;
}
