/**
 * @file cnn.c - 极简CNN: Conv+ReLU+MaxPool+FC, im2col+GEMM卷积
 */
#include "cnn.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <assert.h>

#define C2IDX(oc,ic,kh,kw,IC,KH,KW) ((oc)*(IC)*(KH)*(KW) + (ic)*(KH)*(KW) + (kh)*(KW) + (kw))

/* ===== im2col ===== */
static void _im2col(const float* data_im, int channels, int height, int width,
                    int kH, int kW, int pad, int stride,
                    float* data_col, int out_h, int out_w) {
    int col_h = out_h * out_w;
    int c = 0;
    for (int ic = 0; ic < channels; ic++) {
        for (int kh = 0; kh < kH; kh++) {
            for (int kw = 0; kw < kW; kw++) {
                int im_row = -pad + kh;
                for (int oh = 0; oh < out_h; oh++) {
                    int im_col = -pad + kw;
                    for (int ow = 0; ow < out_w; ow++) {
                        float v = 0;
                        if (im_row >= 0 && im_row < height && im_col >= 0 && im_col < width)
                            v = data_im[(ic*height+im_row)*width + im_col];
                        data_col[oh*out_w + ow + c*col_h] = v;
                        im_col += stride;
                    }
                    im_row += stride;
                }
                c++;
            }
        }
    }
}

static ConvLayer _conv_create(int in_c, int out_c, int kh, int kw, int pad, int stride, int in_h, int in_w) {
    ConvLayer L; memset(&L, 0, sizeof(L));
    L.in_c=in_c; L.out_c=out_c; L.kernel_h=kh; L.kernel_w=kw; L.pad=pad; L.stride=stride;
    L.in_h=in_h; L.in_w=in_w;
    L.out_h = (in_h + 2*pad - kh)/stride + 1;
    L.out_w = (in_w + 2*pad - kw)/stride + 1;
    int nW = out_c*in_c*kh*kw;
    L.W = (float*)calloc(nW, sizeof(float));
    L.b = (float*)calloc(out_c, sizeof(float));
    L.dW = (float*)calloc(nW, sizeof(float));
    L.db = (float*)calloc(out_c, sizeof(float));
    L.VdW=(float*)calloc(nW,sizeof(float)); L.Vdb=(float*)calloc(out_c,sizeof(float));
    L.SdW=(float*)calloc(nW,sizeof(float)); L.Sdb=(float*)calloc(out_c,sizeof(float));
    float std = sqrtf(2.0f/(in_c*kh*kw));
    for (int i=0;i<nW;i++) L.W[i] = random_normal(0, std);
    int col_size = in_c*kh*kw * L.out_h*L.out_w;
    L.col = (float*)malloc(col_size*sizeof(float));
    L.dcol = (float*)malloc(col_size*sizeof(float));
    return L;
}
static void _conv_free(ConvLayer* L) {
    free(L->W); free(L->b); free(L->dW); free(L->db);
    free(L->VdW);free(L->Vdb);free(L->SdW);free(L->Sdb);
    free(L->col); free(L->dcol); free(L->input_cache);
}

static MaxPoolLayer _pool_create(int in_c, int in_h, int in_w, int ph, int pw, int stride) {
    MaxPoolLayer P; memset(&P,0,sizeof(P));
    P.pool_h=ph; P.pool_w=pw; P.stride=stride;
    P.in_c=in_c; P.in_h=in_h; P.in_w=in_w;
    P.out_h = (in_h - ph)/stride + 1;
    P.out_w = (in_w - pw)/stride + 1;
    P.mask = (int*)malloc(in_c * P.out_h * P.out_w * sizeof(int));
    return P;
}
static void _pool_free(MaxPoolLayer* P) { free(P->mask); }

static FCLayer _fc_create(int in_dim, int out_dim) {
    FCLayer f; memset(&f,0,sizeof(f));
    f.in_dim=in_dim; f.out_dim=out_dim;
    f.W = mat_create(in_dim, out_dim);
    f.b = (float*)calloc(out_dim, sizeof(float));
    f.dW = (float*)calloc(in_dim*out_dim, sizeof(float));
    f.db = (float*)calloc(out_dim, sizeof(float));
    init_he(&f.W, in_dim);
    f.VdW = mat_create(in_dim,out_dim); f.Vdb=(float*)calloc(out_dim,sizeof(float));
    f.SdW = mat_create(in_dim,out_dim); f.Sdb=(float*)calloc(out_dim,sizeof(float));
    return f;
}
static void _fc_free(FCLayer* f) {
    mat_free(&f->W); free(f->b); free(f->dW); free(f->db);
    mat_free(&f->Z); mat_free(&f->A); mat_free(&f->A_prev_flat);
    mat_free(&f->VdW); free(f->Vdb); mat_free(&f->SdW); free(f->Sdb);
}

CNN* cnn_create(void) {
    CNN* net = (CNN*)calloc(1,sizeof(CNN));
    net->conv1 = _conv_create(1, 8, 3, 3, 1, 1, 28, 28);
    net->relu1.mask = NULL;
    net->pool1 = _pool_create(8, 28, 28, 2, 2, 2);
    net->conv2 = _conv_create(8, 16, 3, 3, 1, 1, 14, 14);
    net->pool2 = _pool_create(16, 14, 14, 2, 2, 2);
    net->fc1 = _fc_create(784, 128);
    net->relu3.mask = NULL;
    net->dropout_rate = 0.0f;
    net->dropout_mask = (float*)malloc(128*sizeof(float));
    net->fc2 = _fc_create(128, 10);
    net->probs = (float*)calloc(10, sizeof(float));
    net->lr = 0.001f;
    net->beta1 = 0.9f; net->beta2 = 0.999f; net->eps = 1e-8f;
    net->l2_reg = 1e-4f;
    net->t = 0; net->verbose = 1;
    net->conv1_out = (float*)malloc(8*28*28*sizeof(float));
    net->pool1_out = (float*)malloc(8*14*14*sizeof(float));
    net->conv2_out = (float*)malloc(16*14*14*sizeof(float));
    net->pool2_out = (float*)malloc(16*7*7*sizeof(float));
    net->pool2_flat = (float*)malloc(784*sizeof(float));
    net->fc1_out = (float*)malloc(128*sizeof(float));
    net->batch_size = 1;
    return net;
}

void cnn_free(CNN* net) {
    if (!net) return;
    _conv_free(&net->conv1); _conv_free(&net->conv2);
    _pool_free(&net->pool1); _pool_free(&net->pool2);
    free(net->relu1.mask); free(net->relu3.mask);
    free(net->dropout_mask);
    _fc_free(&net->fc1); _fc_free(&net->fc2);
    free(net->probs);
    free(net->conv1_out); free(net->pool1_out);
    free(net->conv2_out); free(net->pool2_out);
    free(net->pool2_flat); free(net->fc1_out);
    free(net);
}

void cnn_set_adam(CNN* net, float lr, float b1, float b2, float eps, float l2) {
    net->lr=lr; net->beta1=b1; net->beta2=b2; net->eps=eps; net->l2_reg=l2;
}
void cnn_set_dropout(CNN* net, float r) { net->dropout_rate = r; }
void cnn_set_verbose(CNN* net, int v) { net->verbose = v; }

static void _conv_forward(ConvLayer* L, const float* inp, float* out) {
    int out_h = L->out_h, out_w = L->out_w;
    int col_h = out_h*out_w;
    int col_w = L->in_c*L->kernel_h*L->kernel_w;
    _im2col(inp, L->in_c, L->in_h, L->in_w, L->kernel_h, L->kernel_w, L->pad, L->stride, L->col, out_h, out_w);
    for (int oc=0; oc<L->out_c; oc++) {
        for (int i=0; i<col_h; i++) out[oc*col_h + i] = L->b[oc];
        for (int k=0; k<col_w; k++) {
            int ic = k/(L->kernel_h*L->kernel_w);
            int rem = k % (L->kernel_h*L->kernel_w);
            int kh = rem/L->kernel_w, kw = rem%L->kernel_w;
            float wk = L->W[C2IDX(oc,ic,kh,kw,L->in_c,L->kernel_h,L->kernel_w)];
            const float* colk = &L->col[k*col_h];
            float* orow = &out[oc*col_h];
            for (int i=0; i<col_h; i++) orow[i] += wk * colk[i];
        }
    }
    if (!L->input_cache) L->input_cache=(float*)malloc(L->in_c*L->in_h*L->in_w*sizeof(float));
    int in_sz = L->in_c*L->in_h*L->in_w;
    memcpy(L->input_cache, inp, in_sz*sizeof(float));
}

static void _relu_mask(float* x, int n, float** mask_out) {
    float* mask = (float*)malloc(n*sizeof(float));
    for (int i=0;i<n;i++) { if(x[i]>0) { mask[i]=1; } else { x[i]=0; mask[i]=0; } }
    free(*mask_out); *mask_out = mask;
}

static void _maxpool_forward(MaxPoolLayer* P, const float* inp, float* out) {
    int oh=P->out_h, ow=P->out_w;
    for (int c=0;c<P->in_c;c++) {
        for (int i=0;i<oh;i++) for (int j=0;j<ow;j++) {
            float bv = -1e30f; int bi=0,bj=0;
            for (int di=0;di<P->pool_h;di++) for(int dj=0;dj<P->pool_w;dj++) {
                int ii=i*P->stride+di, jj=j*P->stride+dj;
                float v = inp[(c*P->in_h+ii)*P->in_w+jj];
                if (v>bv) { bv=v; bi=di; bj=dj; }
            }
            out[(c*oh+i)*ow+j] = bv;
            P->mask[(c*oh+i)*ow+j] = bi*P->pool_w + bj;
        }
    }
}

static float* _forward(CNN* net, const float* img, int training) {
    _conv_forward(&net->conv1, img, net->conv1_out);
    _relu_mask(net->conv1_out, 8*28*28, &net->relu1.mask);
    _maxpool_forward(&net->pool1, net->conv1_out, net->pool1_out);
    _conv_forward(&net->conv2, net->pool1_out, net->conv2_out);
    _relu_mask(net->conv2_out, 16*14*14, &net->relu3.mask);
    _maxpool_forward(&net->pool2, net->conv2_out, net->pool2_out);
    memcpy(net->pool2_flat, net->pool2_out, 784*sizeof(float));
    for (int j=0;j<128;j++){
        float s=net->fc1.b[j];
        for (int k=0;k<784;k++) s+=net->pool2_flat[k] * *mat_at_c(&net->fc1.W,k,j);
        net->fc1_out[j] = s>0?s:0;
    }
    if (training && net->dropout_rate > 0) {
        float scale = 1.0f/(1-net->dropout_rate);
        for (int j=0;j<128;j++) {
            float r = random_uniform(0,1);
            net->dropout_mask[j] = (r > net->dropout_rate) ? scale : 0;
            net->fc1_out[j] *= net->dropout_mask[j];
        }
    } else {
        for (int j=0;j<128;j++) net->dropout_mask[j] = 1.0f;
    }
    for (int j=0;j<10;j++){
        net->probs[j] = net->fc2.b[j];
        for (int k=0;k<128;k++) net->probs[j] += net->fc1_out[k] * *mat_at_c(&net->fc2.W,k,j);
    }
    softmax_row(net->probs, 10);
    return net->probs;
}

int cnn_predict_one(CNN* net, const float* img) {
    float* p = _forward(net, img, 0);
    size_t mi; vec_max(p, 10, &mi);
    return (int)mi;
}

static void _adam_update(float* w, float* VdW, float* SdW, float dW, float lr, float b1, float b2, float eps, float l2, size_t t) {
    float g = dW + l2*(*w);
    *VdW = b1*(*VdW) + (1-b1)*g;
    *SdW = b2*(*SdW) + (1-b2)*g*g;
    float vc = *VdW/(1-powf(b1,(float)t));
    float sc = *SdW/(1-powf(b2,(float)t));
    *w -= lr * vc/(sqrtf(sc)+eps);
}

float cnn_train_one(CNN* net, const float* img, int label) {
    float* p = _forward(net, img, 1);
    float loss = -logf(p[label] + 1e-12f);
    net->t++;

    float d_fc2[10];
    for (int j=0;j<10;j++) d_fc2[j] = p[j] - (j==label?1.0f:0.0f);
    for (int k=0;k<128;k++) for(int j=0;j<10;j++) {
        float dW = net->fc1_out[k] * d_fc2[j];
        _adam_update(mat_at(&net->fc2.W,k,j), mat_at(&net->fc2.VdW,k,j), mat_at(&net->fc2.SdW,k,j),
                     dW, net->lr, net->beta1, net->beta2, net->eps, net->l2_reg, net->t);
    }
    for (int j=0;j<10;j++)
        _adam_update(&net->fc2.b[j], &net->fc2.Vdb[j], &net->fc2.Sdb[j],
                     d_fc2[j], net->lr, net->beta1, net->beta2, net->eps, 0, net->t);

    float d_fc1[128];
    for (int k=0;k<128;k++) {
        float s=0; for (int j=0;j<10;j++) s += d_fc2[j] * *mat_at_c(&net->fc2.W,k,j);
        d_fc1[k] = s * (net->fc1_out[k]>0 ? 1.0f : 0);
        d_fc1[k] *= net->dropout_mask[k];
    }
    for (int ki=0;ki<784;ki++) for(int j=0;j<128;j++) {
        float dW = net->pool2_flat[ki] * d_fc1[j];
        _adam_update(mat_at(&net->fc1.W,ki,j), mat_at(&net->fc1.VdW,ki,j), mat_at(&net->fc1.SdW,ki,j),
                     dW, net->lr, net->beta1, net->beta2, net->eps, net->l2_reg, net->t);
    }
    for (int j=0;j<128;j++)
        _adam_update(&net->fc1.b[j], &net->fc1.Vdb[j], &net->fc1.Sdb[j],
                     d_fc1[j], net->lr, net->beta1, net->beta2, net->eps, 0, net->t);

    float d_flat[784];
    for (int ki=0;ki<784;ki++) {
        float s=0; for (int j=0;j<128;j++) s += d_fc1[j] * *mat_at_c(&net->fc1.W,ki,j);
        d_flat[ki] = s;
    }
    float d_pool2[16*7*7]; memcpy(d_pool2, d_flat, 784*sizeof(float));

    float conv_lr = net->lr * 0.5f;

    /* Pool2 -> d_conv2 */
    float d_conv2[16*14*14]; memset(d_conv2, 0, sizeof(d_conv2));
    for (int c=0;c<16;c++) for(int i=0;i<7;i++) for(int j=0;j<7;j++) {
        int m = net->pool2.mask[(c*7+i)*7+j];
        int mi=m/2, mj=m%2;
        d_conv2[(c*14+i*2+mi)*14 + j*2+mj] = d_pool2[(c*7+i)*7+j];
    }
    for (int i=0;i<16*14*14;i++) if (net->conv2_out[i] <= 0) d_conv2[i]=0;

    /* Conv2梯度 */
    {
        float dW[16*8*3*3]={0}; float db[16]={0};
        for (int oc=0;oc<16;oc++) for(int oh=0;oh<14;oh++) for(int ow=0;ow<14;ow++) {
            float dy = d_conv2[(oc*14+oh)*14+ow]; db[oc] += dy;
            for (int ic=0;ic<8;ic++) for(int kh=0;kh<3;kh++) for(int kw=0;kw<3;kw++) {
                int ih = oh+kh-1, iw = ow+kw-1;
                float v = 0;
                if (ih>=0&&ih<14&&iw>=0&&iw<14) v = net->pool1_out[(ic*14+ih)*14+iw];
                dW[C2IDX(oc,ic,kh,kw,8,3,3)] += dy*v;
            }
        }
        int nW2 = 16*8*3*3;
        for (int i=0;i<nW2;i++)
            _adam_update(&net->conv2.W[i],&net->conv2.VdW[i],&net->conv2.SdW[i],
                         dW[i],conv_lr,net->beta1,net->beta2,net->eps,net->l2_reg,net->t);
        for (int oc=0;oc<16;oc++)
            _adam_update(&net->conv2.b[oc],&net->conv2.Vdb[oc],&net->conv2.Sdb[oc],
                         db[oc],conv_lr,net->beta1,net->beta2,net->eps,0,net->t);
    }

    /* d_pool1 = Conv2反向传播 */
    float d_pool1[8*14*14]; memset(d_pool1,0,sizeof(d_pool1));
    for (int oc=0;oc<16;oc++) for(int oh=0;oh<14;oh++) for(int ow=0;ow<14;ow++) {
        float dy = d_conv2[(oc*14+oh)*14+ow];
        for (int ic=0;ic<8;ic++) for(int kh=0;kh<3;kh++) for(int kw=0;kw<3;kw++) {
            int ih=oh+kh-1,iw=ow+kw-1;
            if (ih>=0&&ih<14&&iw>=0&&iw<14)
                d_pool1[(ic*14+ih)*14+iw] += dy*net->conv2.W[C2IDX(oc,ic,kh,kw,8,3,3)];
        }
    }

    /* Pool1 -> d_conv1 */
    float d_conv1[8*28*28]; memset(d_conv1,0,sizeof(d_conv1));
    for (int c=0;c<8;c++) for(int i=0;i<14;i++) for(int j=0;j<14;j++) {
        int m = net->pool1.mask[(c*14+i)*14+j];
        int mi=m/2, mj=m%2;
        d_conv1[(c*28+i*2+mi)*28 + j*2+mj] = d_pool1[(c*14+i)*14+j];
    }
    for (int i=0;i<8*28*28;i++) if (net->conv1_out[i] <= 0) d_conv1[i]=0;

    /* Conv1梯度 */
    {
        float dW[8*1*3*3]={0}; float db[8]={0};
        for (int oc=0;oc<8;oc++) for(int oh=0;oh<28;oh++) for(int ow=0;ow<28;ow++) {
            float dy = d_conv1[(oc*28+oh)*28+ow]; db[oc] += dy;
            for (int kh=0;kh<3;kh++) for(int kw=0;kw<3;kw++) {
                int ih=oh+kh-1,iw=ow+kw-1;
                float v=0;
                if (ih>=0&&ih<28&&iw>=0&&iw<28) v=img[ih*28+iw];
                dW[C2IDX(oc,0,kh,kw,1,3,3)] += dy*v;
            }
        }
        int nW1=8*1*3*3;
        for (int i=0;i<nW1;i++)
            _adam_update(&net->conv1.W[i],&net->conv1.VdW[i],&net->conv1.SdW[i],
                         dW[i],conv_lr,net->beta1,net->beta2,net->eps,net->l2_reg,net->t);
        for (int oc=0;oc<8;oc++)
            _adam_update(&net->conv1.b[oc],&net->conv1.Vdb[oc],&net->conv1.Sdb[oc],
                         db[oc],conv_lr,net->beta1,net->beta2,net->eps,0,net->t);
    }
    return loss;
}

float cnn_evaluate(CNN* net, const Dataset* ds, size_t max_n) {
    size_t n = max_n>0 && max_n<ds->n ? max_n : ds->n;
    size_t correct = 0;
    for (size_t i=0;i<n;i++) {
        int pred = cnn_predict_one(net, (const float*)mat_at_c(&ds->X,i,0));
        if (pred == ds->labels[i]) correct++;
    }
    return (float)correct/n;
}

void cnn_fit(CNN* net, const Dataset* train, const Dataset* test, int epochs) {
    size_t n = train->n;
    for (int ep=0;ep<epochs;ep++) {
        float total_loss = 0;
        size_t* perm = (size_t*)malloc(n*sizeof(size_t));
        for (size_t i=0;i<n;i++) perm[i]=i;
        for (size_t i=n-1;i>0;i--){
            size_t j = (size_t)(random_uniform(0,1)*(i+1))%(i+1);
            size_t t=perm[i];perm[i]=perm[j];perm[j]=t;
        }
        for (size_t i=0;i<n;i++){
            size_t idx = perm[i];
            float l = cnn_train_one(net, (const float*)mat_at_c(&train->X,idx,0), train->labels[idx]);
            total_loss += l;
            if (net->verbose && i>0 && n>=20 && i%(n/5)==0)
                printf("  [ep %d] %5zu/%zu  avg_loss=%.4f\r", ep+1, i, n, total_loss/i);
        }
        free(perm);
        float train_acc = cnn_evaluate(net, train, n<1000?n:1000);
        float test_acc = test ? cnn_evaluate(net, test, 0) : 0;
        if (net->verbose)
            printf("  [Epoch %2d/%d] loss=%.4f train_acc=%.2f%% test_acc=%.2f%%\n",
                   ep+1, epochs, total_loss/n, train_acc*100, test_acc*100);
    }
}

int cnn_save(const CNN* net, const char* path) {
    FILE* f = fopen(path,"wb");
    if(!f)return -1;
    fprintf(f,"C_AI_CNN_V1\n");
    if (fwrite(net->conv1.W, sizeof(float), 8*1*3*3, f) != 8*1*3*3) { fclose(f); return -1; }
    fwrite(net->conv1.b, sizeof(float), 8, f);
    fwrite(net->conv2.W, sizeof(float), 16*8*3*3, f);
    fwrite(net->conv2.b, sizeof(float), 16, f);
    fwrite(net->fc1.W.data, sizeof(float), 784*128, f);
    fwrite(net->fc1.b, sizeof(float), 128, f);
    fwrite(net->fc2.W.data, sizeof(float), 128*10, f);
    fwrite(net->fc2.b, sizeof(float), 10, f);
    fclose(f); return 0;
}

CNN* cnn_load(const char* path) {
    FILE* f = fopen(path,"rb");
    if(!f)return NULL;
    char buf[64];
    if(!fgets(buf,sizeof(buf),f)){fclose(f);return NULL;}
    CNN* net = cnn_create();
    (void)!fread(net->conv1.W, sizeof(float), 8*1*3*3, f);
    (void)!fread(net->conv1.b, sizeof(float), 8, f);
    (void)!fread(net->conv2.W, sizeof(float), 16*8*3*3, f);
    (void)!fread(net->conv2.b, sizeof(float), 16, f);
    (void)!fread(net->fc1.W.data, sizeof(float), 784*128, f);
    (void)!fread(net->fc1.b, sizeof(float), 128, f);
    (void)!fread(net->fc2.W.data, sizeof(float), 128*10, f);
    (void)!fread(net->fc2.b, sizeof(float), 10, f);
    fclose(f); return net;
}
