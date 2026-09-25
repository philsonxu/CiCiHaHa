#include "model.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <alloca.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void gpt_config_default(GPTConfig *c){
    c->vocab_size=65; c->d_model=64; c->n_heads=4; c->n_layers=4;
    c->block_size=64; c->lr=5e-4f; c->beta1=0.9f; c->beta2=0.999f;
    c->eps=1e-8f; c->weight_decay=0.01f; c->batch_size=32;
}

struct GPTModel {
    GPTConfig cfg;
    GPTParams p;
    int step;
    int B,T,C,V,L,NH,HS,FC;
    // 前/反向缓冲 (一次性分配)
    float *x, *att;
    float *ln1, *ln2, *q, *k, *v, *att_o, *fc;
    float *fin, *logits, *probs;
    float *dx, *dq, *dk, *dv, *datt, *dfc;
    float *dln1, *dln2, *dres, *dfin, *dlogits, *dpre;
    float *ln_mean, *ln_invstd, *ln_xhat;
};

// 注意：简单起见，每次forward前清空所有缓冲指针（模型尺寸固定时这样做没问题）
// 简化为：缓冲指针都在create里按最大尺寸一次性分配——但更简单的方案是，
// gpt_forward_backward 依赖调用方固定cfg，每次不变化。为避免越界，我们在fwd中每次free并重新分配：
// 不重新分配，按create时的尺寸固定（调用者不能中途改变B/T/C）
static float* buf(float **p, size_t n){
    if(!*p) *p=(float*)calloc(n,sizeof(float));
    return *p;
}

static inline float gelu_f(float x){
    float c=sqrtf(2.0f/(float)M_PI);
    return 0.5f*x*(1.0f+tanhf(c*(x+0.044715f*x*x*x)));
}
static inline float gelu_g(float x){
    float c=sqrtf(2.0f/(float)M_PI);
    float t=tanhf(c*(x+0.044715f*x*x*x));
    float dt=c*(1+3*0.044715f*x*x)*(1-t*t);
    return 0.5f*(1+t)+0.5f*x*dt;
}

static void linear_fwd(float *y, const float *x, const float *W, const float *b, int N,int Cin,int Cout){
    for(int i=0;i<N;i++){const float*xi=x+i*Cin;float*yi=y+i*Cout;
        for(int j=0;j<Cout;j++){const float*wj=W+j*Cin;
            float s=b?b[j]:0.0f;
            for(int k=0;k<Cin;k++)s+=xi[k]*wj[k];
            yi[j]=s;}}
}
static void linear_bwd_add(const float *dy, const float *x, const float *W,
                           float *dW, float *db, float *dx, int N,int Cin,int Cout){
    for(int i=0;i<N;i++){const float*dyi=dy+i*Cout;const float*xi=x+i*Cin;
        for(int j=0;j<Cout;j++){float dyij=dyi[j];
            if(db)db[j]+=dyij;
            const float*wj=W+j*Cin;float*dwj=dW+j*Cin;
            float*dxi=dx?dx+i*Cin:NULL;
            for(int k=0;k<Cin;k++){dwj[k]+=dyij*xi[k];if(dxi)dxi[k]+=dyij*wj[k];}
        }}
}

static void ln_fwd(float *out, const float *in, const float *g, const float *b,
                   int N,int C,float eps,float *mean,float *invstd,float *xhat){
    for(int i=0;i<N;i++){const float*xi=in+i*C;float*oi=out+i*C;
        float m=0,v=0;
        for(int j=0;j<C;j++)m+=xi[j];m/=C;
        for(int j=0;j<C;j++){float d=xi[j]-m;v+=d*d;}v/=C;
        float is=1.0f/sqrtf(v+eps);mean[i]=m;invstd[i]=is;
        for(int j=0;j<C;j++){float h=(xi[j]-m)*is;xhat[i*C+j]=h;oi[j]=g[j]*h+b[j];}}
}
static void ln_bwd(const float *dout, const float *xhat, const float *invstd, const float *g,
                   float *dx, float *dg, float *db, int N,int C){
    if(dg)memset(dg,0,C*sizeof(float));if(db)memset(db,0,C*sizeof(float));
    for(int i=0;i<N;i++){const float*di=dout+i*C;
        for(int j=0;j<C;j++){if(db)db[j]+=di[j];if(dg)dg[j]+=di[j]*xhat[i*C+j];}}
    if(!dx)return;
    for(int i=0;i<N;i++){const float*di=dout+i*C;float is=invstd[i];float sum_d=0,sum_dxh=0;
        for(int j=0;j<C;j++){float dh=di[j]*g[j];sum_d+=dh;sum_dxh+=dh*xhat[i*C+j];}
        for(int j=0;j<C;j++){float dh=di[j]*g[j];
            dx[i*C+j]=is*(dh-(sum_d+xhat[i*C+j]*sum_dxh)/C);}}
}

static void attn_fwd(float *out, float *aw, const float *q, const float *k, const float *v,
                     int B,int T,int NH,int HS){
    float sc=1.0f/sqrtf((float)HS);
    for(int b=0;b<B;b++)for(int h=0;h<NH;h++){float*a=aw+(b*NH+h)*T*T;
        for(int t1=0;t1<T;t1++){const float*q1=q+(b*T+t1)*NH*HS+h*HS;
            for(int t2=0;t2<T;t2++){const float*k2=k+(b*T+t2)*NH*HS+h*HS;float s=0;
                for(int d=0;d<HS;d++)s+=q1[d]*k2[d];
                a[t1*T+t2]=s*sc;}
            for(int t2=t1+1;t2<T;t2++)a[t1*T+t2]=-1e9f;}
        for(int t1=0;t1<T;t1++){float*r=a+t1*T;float mx=-FLT_MAX,sum=0;
            for(int t2=0;t2<T;t2++)if(r[t2]>mx)mx=r[t2];
            for(int t2=0;t2<T;t2++){r[t2]=expf(r[t2]-mx);sum+=r[t2];}
            for(int t2=0;t2<T;t2++)r[t2]/=sum;}
        for(int t1=0;t1<T;t1++){float*o=out+(b*T+t1)*NH*HS+h*HS;
            for(int d=0;d<HS;d++)o[d]=0;
            for(int t2=0;t2<T;t2++){float w=a[t1*T+t2];const float*v2=v+(b*T+t2)*NH*HS+h*HS;
                for(int d=0;d<HS;d++)o[d]+=w*v2[d];}}}
}
static void attn_bwd(float *dq,float *dk,float *dv,const float *dout,const float *aw,
                     const float *q,const float *k,const float *v,int B,int T,int NH,int HS){
    float sc=1.0f/sqrtf((float)HS);
    for(int b=0;b<B;b++)for(int h=0;h<NH;h++){const float*a=aw+(b*NH+h)*T*T;
        float*ds=(float*)alloca(T*T*sizeof(float));
        for(int t1=0;t1<T;t1++){const float*do1=dout+(b*T+t1)*NH*HS+h*HS;float*dsr=ds+t1*T;
            for(int t2=0;t2<T;t2++){const float*v2=v+(b*T+t2)*NH*HS+h*HS;float dot=0;
                for(int d=0;d<HS;d++)dot+=do1[d]*v2[d];
                dsr[t2]=dot;}
            float ssum=0;
            for(int t2=0;t2<T;t2++)ssum+=a[t1*T+t2]*dsr[t2];
            for(int t2=0;t2<T;t2++)dsr[t2]=a[t1*T+t2]*(dsr[t2]-ssum);
            for(int t2=t1+1;t2<T;t2++)dsr[t2]=0;}
        for(int t1=0;t1<T;t1++){float*dq1=dq+(b*T+t1)*NH*HS+h*HS;const float*q1=q+(b*T+t1)*NH*HS+h*HS;
            for(int t2=0;t2<T;t2++){float s=ds[t1*T+t2]*sc;
                const float*k2=k+(b*T+t2)*NH*HS+h*HS;float*dk2=dk+(b*T+t2)*NH*HS+h*HS;
                for(int d=0;d<HS;d++){dq1[d]+=s*k2[d];dk2[d]+=s*q1[d];}}}
        for(int t1=0;t1<T;t1++){const float*do1=dout+(b*T+t1)*NH*HS+h*HS;
            for(int t2=0;t2<T;t2++){float w=a[t1*T+t2];float*dv2=dv+(b*T+t2)*NH*HS+h*HS;
                for(int d=0;d<HS;d++)dv2[d]+=w*do1[d];}}}
}

static float softmax_ce(float *probs,const float *logits,float *dlogits,const int *targets,int N,int V){
    for(int i=0;i<N;i++){const float*li=logits+i*V;float*pi=probs+i*V;float m=-FLT_MAX;
        for(int j=0;j<V;j++)if(li[j]>m)m=li[j];
        float s=0;
        for(int j=0;j<V;j++){pi[j]=expf(li[j]-m);s+=pi[j];}
        for(int j=0;j<V;j++)pi[j]/=s;}
    float loss=0;int cnt=0;
    for(int i=0;i<N*V;i++)dlogits[i]=0;
    for(int i=0;i<N;i++){int t=targets[i];if(t<0)continue;cnt++;
        float p=probs[i*V+t];if(p<1e-12f)p=1e-12f;loss+=-logf(p);
        for(int j=0;j<V;j++)dlogits[i*V+j]=probs[i*V+j];
        dlogits[i*V+t]-=1.0f;}
    if(cnt>0){loss/=cnt;float inv=1.0f/cnt;for(int i=0;i<N*V;i++)dlogits[i]*=inv;}
    return loss;
}

GPTModel* gpt_create(const GPTConfig *cfg){
    GPTModel *m=(GPTModel*)calloc(1,sizeof(GPTModel));
    m->cfg=*cfg;
    int V=cfg->vocab_size,C=cfg->d_model,T=cfg->block_size,L=cfg->n_layers,F=4*C;
    m->B=cfg->batch_size;m->T=T;m->C=C;m->V=V;m->L=L;m->NH=cfg->n_heads;m->HS=C/m->NH;m->FC=F;
    #define AL(name,sz) m->p.name=(float*)calloc(sz,sizeof(float));m->p.d##name=(float*)calloc(sz,sizeof(float));m->p.m##name=(float*)calloc(sz,sizeof(float));m->p.v##name=(float*)calloc(sz,sizeof(float))
    AL(wte,(size_t)V*C);AL(wpe,(size_t)T*C);
    AL(ln1g,(size_t)L*C);AL(ln1b,(size_t)L*C);
    AL(wq,(size_t)L*C*C);AL(wk,(size_t)L*C*C);AL(wv,(size_t)L*C*C);AL(wo,(size_t)L*C*C);
    AL(bq,(size_t)L*C);AL(bk,(size_t)L*C);AL(bv,(size_t)L*C);AL(bo,(size_t)L*C);
    AL(ln2g,(size_t)L*C);AL(ln2b,(size_t)L*C);
    AL(wfc,(size_t)L*F*C);AL(bfc,(size_t)L*F);
    AL(wproj,(size_t)L*C*F);AL(bproj,(size_t)L*C);
    AL(lnfg,C);AL(lnfb,C);
    #undef AL
    float sw=0.02f;
    for(size_t i=0;i<(size_t)V*C;i++)m->p.wte[i]=cllm_rand_normal(0,sw);
    for(size_t i=0;i<(size_t)T*C;i++)m->p.wpe[i]=cllm_rand_normal(0,sw);
    for(int l=0;l<L;l++){
        for(int i=0;i<C;i++){
            m->p.ln1g[l*C+i]=1;m->p.ln1b[l*C+i]=0;
            m->p.ln2g[l*C+i]=1;m->p.ln2b[l*C+i]=0;}
        float *q=m->p.wq+l*C*C,*k=m->p.wk+l*C*C,*v=m->p.wv+l*C*C,*o=m->p.wo+l*C*C;
        for(size_t i=0;i<(size_t)C*C;i++){
            q[i]=cllm_rand_normal(0,sw);k[i]=cllm_rand_normal(0,sw);v[i]=cllm_rand_normal(0,sw);
            o[i]=cllm_rand_normal(0,sw/sqrtf((float)L));}
        for(int i=0;i<C;i++){m->p.bq[l*C+i]=0;m->p.bk[l*C+i]=0;m->p.bv[l*C+i]=0;m->p.bo[l*C+i]=0;}
        float *fc=m->p.wfc+l*F*C,*bf=m->p.bfc+l*F;
        float *pr=m->p.wproj+l*C*F,*bp=m->p.bproj+l*C;
        for(size_t i=0;i<(size_t)F*C;i++)fc[i]=cllm_rand_normal(0,sw*sqrtf(2.0f));
        for(int i=0;i<F;i++)bf[i]=0;
        for(size_t i=0;i<(size_t)C*F;i++)pr[i]=cllm_rand_normal(0,sw/sqrtf(2.0f*L));
        for(int i=0;i<C;i++)bp[i]=0;
    }
    for(int i=0;i<C;i++){m->p.lnfg[i]=1;m->p.lnfb[i]=0;}
    return m;
}

void gpt_free(GPTModel *m){
    if(!m)return;
    #define FR(name) free(m->p.name);free(m->p.d##name);free(m->p.m##name);free(m->p.v##name)
    FR(wte);FR(wpe);FR(ln1g);FR(ln1b);
    FR(wq);FR(wk);FR(wv);FR(wo);FR(bq);FR(bk);FR(bv);FR(bo);
    FR(ln2g);FR(ln2b);FR(wfc);FR(bfc);FR(wproj);FR(bproj);FR(lnfg);FR(lnfb);
    #undef FR
    free(m->x);free(m->att);free(m->ln1);free(m->ln2);free(m->q);free(m->k);free(m->v);
    free(m->att_o);free(m->fc);free(m->fin);free(m->logits);free(m->probs);
    free(m->dx);free(m->dq);free(m->dk);free(m->dv);free(m->dfc);
    free(m->dln1);free(m->dln2);free(m->dres);free(m->dfin);free(m->dlogits);free(m->dpre);
    free(m->ln_mean);free(m->ln_invstd);free(m->ln_xhat);
    free(m);
}
long long gpt_param_count(const GPTModel *m){
    int V=m->cfg.vocab_size,C=m->cfg.d_model,T=m->cfg.block_size,L=m->cfg.n_layers,F=4*C;
    long long n=(long long)V*C+(long long)T*C;
    n+=L*(2*C+4*C*C+4*C+2*C+F*C+F+C*F+C)+2*C;
    return n;
}
GPTConfig gpt_config(const GPTModel *m){return m->cfg;}

// ===== Forward+Backward =====
float gpt_forward_backward(GPTModel *m, const int *inputs, const int *targets){
    int B=m->B,T=m->T,C=m->C,V=m->V,L=m->cfg.n_layers,NH=m->NH,HS=m->HS,FC=m->FC;
    int BT=B*T;
    float *x      =buf(&m->x,(size_t)BT*C);
    float *att    =buf(&m->att,(size_t)B*NH*T*T);
    float *ln1    =buf(&m->ln1,(size_t)BT*C);
    float *ln2    =buf(&m->ln2,(size_t)BT*C);
    float *q      =buf(&m->q,(size_t)BT*C);
    float *k      =buf(&m->k,(size_t)BT*C);
    float *v      =buf(&m->v,(size_t)BT*C);
    float *ao     =buf(&m->att_o,(size_t)BT*C);
    float *fc     =buf(&m->fc,(size_t)BT*FC);
    float *fin    =buf(&m->fin,(size_t)BT*C);
    float *logits =buf(&m->logits,(size_t)BT*V);
    float *probs  =buf(&m->probs,(size_t)BT*V);
    float *mean   =buf(&m->ln_mean,(size_t)L*BT*2);
    float *invstd =buf(&m->ln_invstd,(size_t)L*BT*2);
    float *xhat   =buf(&m->ln_xhat,(size_t)L*BT*C*2+BT*C);
    float *dx     =buf(&m->dx,(size_t)BT*C);
    float *dq     =buf(&m->dq,(size_t)BT*C);
    float *dk     =buf(&m->dk,(size_t)BT*C);
    float *dv     =buf(&m->dv,(size_t)BT*C);
    float *dfc    =buf(&m->dfc,(size_t)BT*FC);
    float *dln1   =buf(&m->dln1,(size_t)BT*C);
    float *dln2   =buf(&m->dln2,(size_t)BT*C);
    float *dres   =buf(&m->dres,(size_t)BT*C);
    float *dfin   =buf(&m->dfin,(size_t)BT*C);
    float *dlogits=buf(&m->dlogits,(size_t)BT*V);
    float *dpre   =buf(&m->dpre,(size_t)BT*FC);
    // 逐层激活保存
    float *ln1_in =(float*)malloc((size_t)L*BT*C*sizeof(float));
    float *ln2_in =(float*)malloc((size_t)L*BT*C*sizeof(float));
    float *fc_all =(float*)malloc((size_t)L*BT*FC*sizeof(float));
    float *pre_all=(float*)malloc((size_t)L*BT*FC*sizeof(float));
    float *q_all  =(float*)malloc((size_t)L*BT*C*sizeof(float));
    float *k_all  =(float*)malloc((size_t)L*BT*C*sizeof(float));
    float *v_all  =(float*)malloc((size_t)L*BT*C*sizeof(float));
    float *att_all=(float*)malloc((size_t)L*B*NH*T*T*sizeof(float));
    float *ao_all =(float*)malloc((size_t)L*BT*C*sizeof(float));
    float *l1o_all=(float*)malloc((size_t)L*BT*C*sizeof(float));
    float *l2o_all=(float*)malloc((size_t)L*BT*C*sizeof(float));
    float *m1_all =(float*)malloc((size_t)L*BT*sizeof(float));
    float *i1_all =(float*)malloc((size_t)L*BT*sizeof(float));
    float *x1_all =(float*)malloc((size_t)L*BT*C*sizeof(float));
    float *m2_all =(float*)malloc((size_t)L*BT*sizeof(float));
    float *i2_all =(float*)malloc((size_t)L*BT*sizeof(float));
    float *x2_all =(float*)malloc((size_t)L*BT*C*sizeof(float));

    // 零权重梯度
    #define ZP(name,n) memset(m->p.d##name,0,(n)*sizeof(float))
    ZP(wte,(size_t)V*C);ZP(wpe,(size_t)T*C);
    ZP(ln1g,(size_t)L*C);ZP(ln1b,(size_t)L*C);
    ZP(wq,(size_t)L*C*C);ZP(wk,(size_t)L*C*C);ZP(wv,(size_t)L*C*C);ZP(wo,(size_t)L*C*C);
    ZP(bq,(size_t)L*C);ZP(bk,(size_t)L*C);ZP(bv,(size_t)L*C);ZP(bo,(size_t)L*C);
    ZP(ln2g,(size_t)L*C);ZP(ln2b,(size_t)L*C);
    ZP(wfc,(size_t)L*FC*C);ZP(bfc,(size_t)L*FC);ZP(wproj,(size_t)L*C*FC);ZP(bproj,(size_t)L*C);
    ZP(lnfg,C);ZP(lnfb,C);
    #undef ZP

    // Embedding
    for(int b=0;b<B;b++)for(int t=0;t<T;t++){
        int tok=inputs[b*T+t];float*xr=x+(b*T+t)*C;
        const float*te=m->p.wte+tok*C;const float*pe=m->p.wpe+t*C;
        for(int i=0;i<C;i++)xr[i]=te[i]+pe[i];}
    // Blocks fwd
    for(int l=0;l<L;l++){
        float*li=ln1_in+(size_t)l*BT*C;memcpy(li,x,BT*C*sizeof(float));
        float*l1o=l1o_all+(size_t)l*BT*C;
        ln_fwd(l1o,x,m->p.ln1g+l*C,m->p.ln1b+l*C,BT,C,1e-5f,m1_all+(size_t)l*BT,i1_all+(size_t)l*BT,x1_all+(size_t)l*BT*C);
        float*ql=q_all+(size_t)l*BT*C,*kl=k_all+(size_t)l*BT*C,*vl=v_all+(size_t)l*BT*C;
        linear_fwd(ql,l1o,m->p.wq+l*C*C,m->p.bq+l*C,BT,C,C);
        linear_fwd(kl,l1o,m->p.wk+l*C*C,m->p.bk+l*C,BT,C,C);
        linear_fwd(vl,l1o,m->p.wv+l*C*C,m->p.bv+l*C,BT,C,C);
        float*al=att_all+(size_t)l*B*NH*T*T;float*ao_l=ao_all+(size_t)l*BT*C;
        attn_fwd(ao_l,al,ql,kl,vl,B,T,NH,HS);
        linear_fwd(ln1,ao_l,m->p.wo+l*C*C,m->p.bo+l*C,BT,C,C);
        for(int i=0;i<BT*C;i++)x[i]=li[i]+ln1[i];
        float*li2=ln2_in+(size_t)l*BT*C;memcpy(li2,x,BT*C*sizeof(float));
        float*l2o=l2o_all+(size_t)l*BT*C;
        ln_fwd(l2o,x,m->p.ln2g+l*C,m->p.ln2b+l*C,BT,C,1e-5f,m2_all+(size_t)l*BT,i2_all+(size_t)l*BT,x2_all+(size_t)l*BT*C);
        float*fcl=fc_all+(size_t)l*BT*FC;float*pre=pre_all+(size_t)l*BT*FC;
        linear_fwd(pre,l2o,m->p.wfc+l*FC*C,m->p.bfc+l*FC,BT,C,FC);
        for(int i=0;i<BT*FC;i++)fcl[i]=gelu_f(pre[i]);
        linear_fwd(ln2,fcl,m->p.wproj+l*C*FC,m->p.bproj+l*C,BT,FC,C);
        for(int i=0;i<BT*C;i++)x[i]=li2[i]+ln2[i];
    }
    // Final LN
    float *fm=mean+L*BT*2,*fi=invstd+L*BT*2,*fx=xhat+L*BT*C*2;
    ln_fwd(fin,x,m->p.lnfg,m->p.lnfb,BT,C,1e-5f,fm,fi,fx);
    // LM head
    for(int i=0;i<BT;i++){const float*fi2=fin+i*C;float*li=logits+i*V;
        for(int j=0;j<V;j++){const float*wj=m->p.wte+j*C;float s=0;
            for(int k=0;k<C;k++)s+=fi2[k]*wj[k];
            li[j]=s;}}
    float loss=softmax_ce(probs,logits,dlogits,targets,BT,V);
    // Bwd LM head -> dfin
    for(int i=0;i<BT*C;i++)dfin[i]=0;
    for(int i=0;i<BT;i++){const float*dli=dlogits+i*V;float*dfi=dfin+i*C;
        for(int j=0;j<V;j++){float dl=dli[j];const float*wj=m->p.wte+j*C;
            float*dwj=m->p.dwte+j*C;const float*fi2=fin+i*C;
            for(int k=0;k<C;k++){dfi[k]+=dl*wj[k];dwj[k]+=dl*fi2[k];}}}
    memset(dx,0,BT*C*sizeof(float));
    ln_bwd(dfin,fx,fi,m->p.lnfg,dx,m->p.dlnfg,m->p.dlnfb,BT,C);
    for(int l=L-1;l>=0;l--){
        float*li=ln1_in+(size_t)l*BT*C; (void)li;
        float*li2=ln2_in+(size_t)l*BT*C;
        float*l1o=l1o_all+(size_t)l*BT*C,*l2o=l2o_all+(size_t)l*BT*C;
        float*ql=q_all+(size_t)l*BT*C,*kl=k_all+(size_t)l*BT*C,*vl=v_all+(size_t)l*BT*C;
        float*al=att_all+(size_t)l*B*NH*T*T,*ao_l=ao_all+(size_t)l*BT*C;
        float*fcl=fc_all+(size_t)l*BT*FC,*pre=pre_all+(size_t)l*BT*FC;
        float*dmlp=dln2;memcpy(dmlp,dx,BT*C*sizeof(float));
        float*dln2_in=dx;
        memset(dfc,0,(size_t)BT*FC*sizeof(float));
        linear_bwd_add(dmlp,fcl,m->p.wproj+l*C*FC,m->p.dwproj+l*C*FC,m->p.dbproj+l*C,dfc,BT,FC,C);
        for(int i=0;i<BT*FC;i++)dpre[i]=dfc[i]*gelu_g(pre[i]);
        memset(dln2,0,BT*C*sizeof(float));
        linear_bwd_add(dpre,l2o,m->p.wfc+l*FC*C,m->p.dwfc+l*FC*C,m->p.dbfc+l*FC,dln2,BT,C,FC);
        float*dx_after_attn=dres;memset(dx_after_attn,0,BT*C*sizeof(float));
        ln_bwd(dln2,x2_all+(size_t)l*BT*C,i2_all+(size_t)l*BT,m->p.ln2g+l*C,
               dx_after_attn,m->p.dln2g+l*C,m->p.dln2b+l*C,BT,C);
        for(int i=0;i<BT*C;i++)dln2_in[i]+=dx_after_attn[i];
        float*dproj=dln2_in;float*dres1=dres;memcpy(dres1,dproj,BT*C*sizeof(float));(void)dres1;
        float*dao=dfc;memset(dao,0,BT*C*sizeof(float)); // 复用dfc (FC>C够大)
        linear_bwd_add(dproj,ao_l,m->p.wo+l*C*C,m->p.dwo+l*C*C,m->p.dbo+l*C,dao,BT,C,C);
        memset(dq,0,BT*C*sizeof(float));memset(dk,0,BT*C*sizeof(float));memset(dv,0,BT*C*sizeof(float));
        attn_bwd(dq,dk,dv,dao,al,ql,kl,vl,B,T,NH,HS);
        float*dl1o=dln1;memset(dl1o,0,BT*C*sizeof(float));
        linear_bwd_add(dq,l1o,m->p.wq+l*C*C,m->p.dwq+l*C*C,m->p.dbq+l*C,dl1o,BT,C,C);
        linear_bwd_add(dk,l1o,m->p.wk+l*C*C,m->p.dwk+l*C*C,m->p.dbk+l*C,dl1o,BT,C,C);
        linear_bwd_add(dv,l1o,m->p.wv+l*C*C,m->p.dwv+l*C*C,m->p.dbv+l*C,dl1o,BT,C,C);
        float*dx_out=dx;memset(dx_out,0,BT*C*sizeof(float));
        ln_bwd(dl1o,x1_all+(size_t)l*BT*C,i1_all+(size_t)l*BT,m->p.ln1g+l*C,
               dx_out,m->p.dln1g+l*C,m->p.dln1b+l*C,BT,C);
        for(int i=0;i<BT*C;i++)dx_out[i]+=dproj[i]; // dres1 = dproj
    }
    // Embedding bwd
    for(int b=0;b<B;b++)for(int t=0;t<T;t++){int tok=inputs[b*T+t];
        const float*dxi=dx+(b*T+t)*C;float*dwte=m->p.dwte+tok*C;float*dwpe=m->p.dwpe+t*C;
        for(int i=0;i<C;i++){dwte[i]+=dxi[i];dwpe[i]+=dxi[i];}}
    free(ln1_in);free(ln2_in);free(fc_all);free(pre_all);
    free(q_all);free(k_all);free(v_all);free(att_all);free(ao_all);
    free(l1o_all);free(l2o_all);free(m1_all);free(i1_all);free(x1_all);
    free(m2_all);free(i2_all);free(x2_all);
    return loss;
}

// ===== AdamW =====
void gpt_step(GPTModel *m) {
    m->step++;
    int t=m->step;
    int V=m->cfg.vocab_size,C=m->cfg.d_model,T=m->cfg.block_size,L=m->cfg.n_layers,F=4*C;
    float lr=m->cfg.lr,b1=m->cfg.beta1,b2=m->cfg.beta2,eps=m->cfg.eps,wd=m->cfg.weight_decay;
    float bc1=1.0f-powf(b1,(float)t),bc2=1.0f-powf(b2,(float)t);
    {size_t _n=(size_t)(V*C);for(size_t _i=0;_i<_n;_i++){float g=m->p.dwte[_i];if(1)g+=wd*m->p.wte[_i];m->p.mwte[_i]=b1*m->p.mwte[_i]+(1-b1)*g;m->p.vwte[_i]=b2*m->p.vwte[_i]+(1-b2)*g*g;float _mh=m->p.mwte[_i]/bc1,_vh=m->p.vwte[_i]/bc2;m->p.wte[_i]-=lr*_mh/(sqrtf(_vh)+eps);}}
    {size_t _n=(size_t)(T*C);for(size_t _i=0;_i<_n;_i++){float g=m->p.dwpe[_i];if(1)g+=wd*m->p.wpe[_i];m->p.mwpe[_i]=b1*m->p.mwpe[_i]+(1-b1)*g;m->p.vwpe[_i]=b2*m->p.vwpe[_i]+(1-b2)*g*g;float _mh=m->p.mwpe[_i]/bc1,_vh=m->p.vwpe[_i]/bc2;m->p.wpe[_i]-=lr*_mh/(sqrtf(_vh)+eps);}}
    {size_t _n=(size_t)(L*C);for(size_t _i=0;_i<_n;_i++){float g=m->p.dln1g[_i];if(0)g+=wd*m->p.ln1g[_i];m->p.mln1g[_i]=b1*m->p.mln1g[_i]+(1-b1)*g;m->p.vln1g[_i]=b2*m->p.vln1g[_i]+(1-b2)*g*g;float _mh=m->p.mln1g[_i]/bc1,_vh=m->p.vln1g[_i]/bc2;m->p.ln1g[_i]-=lr*_mh/(sqrtf(_vh)+eps);}}
    {size_t _n=(size_t)(L*C);for(size_t _i=0;_i<_n;_i++){float g=m->p.dln1b[_i];if(0)g+=wd*m->p.ln1b[_i];m->p.mln1b[_i]=b1*m->p.mln1b[_i]+(1-b1)*g;m->p.vln1b[_i]=b2*m->p.vln1b[_i]+(1-b2)*g*g;float _mh=m->p.mln1b[_i]/bc1,_vh=m->p.vln1b[_i]/bc2;m->p.ln1b[_i]-=lr*_mh/(sqrtf(_vh)+eps);}}
    {size_t _n=(size_t)(L*C*C);for(size_t _i=0;_i<_n;_i++){float g=m->p.dwq[_i];if(1)g+=wd*m->p.wq[_i];m->p.mwq[_i]=b1*m->p.mwq[_i]+(1-b1)*g;m->p.vwq[_i]=b2*m->p.vwq[_i]+(1-b2)*g*g;float _mh=m->p.mwq[_i]/bc1,_vh=m->p.vwq[_i]/bc2;m->p.wq[_i]-=lr*_mh/(sqrtf(_vh)+eps);}}
    {size_t _n=(size_t)(L*C*C);for(size_t _i=0;_i<_n;_i++){float g=m->p.dwk[_i];if(1)g+=wd*m->p.wk[_i];m->p.mwk[_i]=b1*m->p.mwk[_i]+(1-b1)*g;m->p.vwk[_i]=b2*m->p.vwk[_i]+(1-b2)*g*g;float _mh=m->p.mwk[_i]/bc1,_vh=m->p.vwk[_i]/bc2;m->p.wk[_i]-=lr*_mh/(sqrtf(_vh)+eps);}}
    {size_t _n=(size_t)(L*C*C);for(size_t _i=0;_i<_n;_i++){float g=m->p.dwv[_i];if(1)g+=wd*m->p.wv[_i];m->p.mwv[_i]=b1*m->p.mwv[_i]+(1-b1)*g;m->p.vwv[_i]=b2*m->p.vwv[_i]+(1-b2)*g*g;float _mh=m->p.mwv[_i]/bc1,_vh=m->p.vwv[_i]/bc2;m->p.wv[_i]-=lr*_mh/(sqrtf(_vh)+eps);}}
    {size_t _n=(size_t)(L*C*C);for(size_t _i=0;_i<_n;_i++){float g=m->p.dwo[_i];if(1)g+=wd*m->p.wo[_i];m->p.mwo[_i]=b1*m->p.mwo[_i]+(1-b1)*g;m->p.vwo[_i]=b2*m->p.vwo[_i]+(1-b2)*g*g;float _mh=m->p.mwo[_i]/bc1,_vh=m->p.vwo[_i]/bc2;m->p.wo[_i]-=lr*_mh/(sqrtf(_vh)+eps);}}
    {size_t _n=(size_t)(L*C);for(size_t _i=0;_i<_n;_i++){float g=m->p.dbq[_i];if(0)g+=wd*m->p.bq[_i];m->p.mbq[_i]=b1*m->p.mbq[_i]+(1-b1)*g;m->p.vbq[_i]=b2*m->p.vbq[_i]+(1-b2)*g*g;float _mh=m->p.mbq[_i]/bc1,_vh=m->p.vbq[_i]/bc2;m->p.bq[_i]-=lr*_mh/(sqrtf(_vh)+eps);}}
    {size_t _n=(size_t)(L*C);for(size_t _i=0;_i<_n;_i++){float g=m->p.dbk[_i];if(0)g+=wd*m->p.bk[_i];m->p.mbk[_i]=b1*m->p.mbk[_i]+(1-b1)*g;m->p.vbk[_i]=b2*m->p.vbk[_i]+(1-b2)*g*g;float _mh=m->p.mbk[_i]/bc1,_vh=m->p.vbk[_i]/bc2;m->p.bk[_i]-=lr*_mh/(sqrtf(_vh)+eps);}}
    {size_t _n=(size_t)(L*C);for(size_t _i=0;_i<_n;_i++){float g=m->p.dbv[_i];if(0)g+=wd*m->p.bv[_i];m->p.mbv[_i]=b1*m->p.mbv[_i]+(1-b1)*g;m->p.vbv[_i]=b2*m->p.vbv[_i]+(1-b2)*g*g;float _mh=m->p.mbv[_i]/bc1,_vh=m->p.vbv[_i]/bc2;m->p.bv[_i]-=lr*_mh/(sqrtf(_vh)+eps);}}
    {size_t _n=(size_t)(L*C);for(size_t _i=0;_i<_n;_i++){float g=m->p.dbo[_i];if(0)g+=wd*m->p.bo[_i];m->p.mbo[_i]=b1*m->p.mbo[_i]+(1-b1)*g;m->p.vbo[_i]=b2*m->p.vbo[_i]+(1-b2)*g*g;float _mh=m->p.mbo[_i]/bc1,_vh=m->p.vbo[_i]/bc2;m->p.bo[_i]-=lr*_mh/(sqrtf(_vh)+eps);}}
    {size_t _n=(size_t)(L*C);for(size_t _i=0;_i<_n;_i++){float g=m->p.dln2g[_i];if(0)g+=wd*m->p.ln2g[_i];m->p.mln2g[_i]=b1*m->p.mln2g[_i]+(1-b1)*g;m->p.vln2g[_i]=b2*m->p.vln2g[_i]+(1-b2)*g*g;float _mh=m->p.mln2g[_i]/bc1,_vh=m->p.vln2g[_i]/bc2;m->p.ln2g[_i]-=lr*_mh/(sqrtf(_vh)+eps);}}
    {size_t _n=(size_t)(L*C);for(size_t _i=0;_i<_n;_i++){float g=m->p.dln2b[_i];if(0)g+=wd*m->p.ln2b[_i];m->p.mln2b[_i]=b1*m->p.mln2b[_i]+(1-b1)*g;m->p.vln2b[_i]=b2*m->p.vln2b[_i]+(1-b2)*g*g;float _mh=m->p.mln2b[_i]/bc1,_vh=m->p.vln2b[_i]/bc2;m->p.ln2b[_i]-=lr*_mh/(sqrtf(_vh)+eps);}}
    {size_t _n=(size_t)(L*F*C);for(size_t _i=0;_i<_n;_i++){float g=m->p.dwfc[_i];if(1)g+=wd*m->p.wfc[_i];m->p.mwfc[_i]=b1*m->p.mwfc[_i]+(1-b1)*g;m->p.vwfc[_i]=b2*m->p.vwfc[_i]+(1-b2)*g*g;float _mh=m->p.mwfc[_i]/bc1,_vh=m->p.vwfc[_i]/bc2;m->p.wfc[_i]-=lr*_mh/(sqrtf(_vh)+eps);}}
    {size_t _n=(size_t)(L*F);for(size_t _i=0;_i<_n;_i++){float g=m->p.dbfc[_i];if(0)g+=wd*m->p.bfc[_i];m->p.mbfc[_i]=b1*m->p.mbfc[_i]+(1-b1)*g;m->p.vbfc[_i]=b2*m->p.vbfc[_i]+(1-b2)*g*g;float _mh=m->p.mbfc[_i]/bc1,_vh=m->p.vbfc[_i]/bc2;m->p.bfc[_i]-=lr*_mh/(sqrtf(_vh)+eps);}}
    {size_t _n=(size_t)(L*C*F);for(size_t _i=0;_i<_n;_i++){float g=m->p.dwproj[_i];if(1)g+=wd*m->p.wproj[_i];m->p.mwproj[_i]=b1*m->p.mwproj[_i]+(1-b1)*g;m->p.vwproj[_i]=b2*m->p.vwproj[_i]+(1-b2)*g*g;float _mh=m->p.mwproj[_i]/bc1,_vh=m->p.vwproj[_i]/bc2;m->p.wproj[_i]-=lr*_mh/(sqrtf(_vh)+eps);}}
    {size_t _n=(size_t)(L*C);for(size_t _i=0;_i<_n;_i++){float g=m->p.dbproj[_i];if(0)g+=wd*m->p.bproj[_i];m->p.mbproj[_i]=b1*m->p.mbproj[_i]+(1-b1)*g;m->p.vbproj[_i]=b2*m->p.vbproj[_i]+(1-b2)*g*g;float _mh=m->p.mbproj[_i]/bc1,_vh=m->p.vbproj[_i]/bc2;m->p.bproj[_i]-=lr*_mh/(sqrtf(_vh)+eps);}}
    {size_t _n=(size_t)(C);for(size_t _i=0;_i<_n;_i++){float g=m->p.dlnfg[_i];if(0)g+=wd*m->p.lnfg[_i];m->p.mlnfg[_i]=b1*m->p.mlnfg[_i]+(1-b1)*g;m->p.vlnfg[_i]=b2*m->p.vlnfg[_i]+(1-b2)*g*g;float _mh=m->p.mlnfg[_i]/bc1,_vh=m->p.vlnfg[_i]/bc2;m->p.lnfg[_i]-=lr*_mh/(sqrtf(_vh)+eps);}}
    {size_t _n=(size_t)(C);for(size_t _i=0;_i<_n;_i++){float g=m->p.dlnfb[_i];if(0)g+=wd*m->p.lnfb[_i];m->p.mlnfb[_i]=b1*m->p.mlnfb[_i]+(1-b1)*g;m->p.vlnfb[_i]=b2*m->p.vlnfb[_i]+(1-b2)*g*g;float _mh=m->p.mlnfb[_i]/bc1,_vh=m->p.vlnfb[_i]/bc2;m->p.lnfb[_i]-=lr*_mh/(sqrtf(_vh)+eps);}}
}
// ===== 自回归采样（B=1，增量推理） =====
static float* _buf(float **p, size_t n){ if(!*p)*p=(float*)calloc(n,sizeof(float)); return *p; }

void gpt_generate(GPTModel *m, const int *prompt, int plen, int *out, int n_new, float temp) {
    int C=m->C,T=m->T,V=m->V,L=m->cfg.n_layers,NH=m->NH,HS=m->HS,FC=m->FC;
    for(int i=0;i<plen;i++)out[i]=prompt[i];
    // 缓冲（静态，复用）
    static float *x=NULL, *ln1=NULL, *q=NULL, *k=NULL, *v=NULL, *att=NULL, *ao=NULL;
    static float *fc=NULL, *ln2=NULL, *fin=NULL, *logits=NULL;
    x   =_buf(&x,(size_t)T*C);
    ln1 =_buf(&ln1,(size_t)T*C);
    q   =_buf(&q,(size_t)T*C);
    k   =_buf(&k,(size_t)T*C);
    v   =_buf(&v,(size_t)T*C);
    att =_buf(&att,(size_t)NH*T*T);
    ao  =_buf(&ao,(size_t)T*C);
    fc  =_buf(&fc,(size_t)T*FC);
    ln2 =_buf(&ln2,(size_t)T*C);
    fin =_buf(&fin,(size_t)T*C);
    logits=_buf(&logits,V);
    float mean[256],is[256],xh[256*64];
    int total=plen+n_new;
    for(int pos=0;pos<total;pos++){
        int eff = pos+1;
        int off = 0;
        if (eff > T) { eff=T; off=pos-T+1; }
        // Embedding
        for(int t=0;t<eff;t++){
            int tok = out[t+off];
            const float* te=m->p.wte+tok*C;
            const float* pe=m->p.wpe+t*C;
            float* xr=x+t*C;
            for(int i=0;i<C;i++)xr[i]=te[i]+pe[i];
        }
        // Blocks
        for(int l=0;l<L;l++){
            // LN1
            for(int i=0;i<eff;i++){
                const float* xi=x+i*C; float* oi=ln1+i*C;
                float mm=0,vv=0;
                for(int j=0;j<C;j++)mm+=xi[j];mm/=C;
                for(int j=0;j<C;j++){float d=xi[j]-mm;vv+=d*d;}vv/=C;
                float iss=1.0f/sqrtf(vv+1e-5f);
                mean[i]=mm;is[i]=iss;
                const float* g=m->p.ln1g+l*C;const float* b=m->p.ln1b+l*C;
                for(int j=0;j<C;j++){float h=(xi[j]-mm)*iss;xh[i*C+j]=h;oi[j]=g[j]*h+b[j];}
            }
            // QKV
            for(int i=0;i<eff;i++){
                const float* xi=ln1+i*C;
                float* qi=q+i*C;float* ki=k+i*C;float* vi=v+i*C;
                const float* wq=m->p.wq+l*C*C;const float* wkv[2]={m->p.wk+l*C*C,m->p.wv+l*C*C};
                const float* bq=m->p.bq+l*C;const float* bk=m->p.bk+l*C;const float* bv=m->p.bv+l*C;
                for(int j=0;j<C;j++){
                    float sq=bq[j],sk=bk[j],sv=bv[j];
                    const float* wqj=wq+j*C;const float* wkj=wkv[0]+j*C;const float* wvj=wkv[1]+j*C;
                    for(int kk=0;kk<C;kk++){sq+=xi[kk]*wqj[kk];sk+=xi[kk]*wkj[kk];sv+=xi[kk]*wvj[kk];}
                    qi[j]=sq;ki[j]=sk;vi[j]=sv;
                }
            }
            // Attn
            float sc=1.0f/sqrtf((float)HS);
            for(int h=0;h<NH;h++)for(int t1=0;t1<eff;t1++){
                float* a=att+(h*eff+t1)*eff;
                const float* q1=q+t1*C+h*HS;
                for(int t2=0;t2<eff;t2++){
                    const float* k2=k+t2*C+h*HS;float s=0;
                    for(int d=0;d<HS;d++)s+=q1[d]*k2[d];
                    a[t2]=s*sc;
                }
                for(int t2=t1+1;t2<eff;t2++)a[t2]=-1e9f;
                float mx=-1e9f,sum=0;
                for(int t2=0;t2<eff;t2++)if(a[t2]>mx)mx=a[t2];
                for(int t2=0;t2<eff;t2++){a[t2]=expf(a[t2]-mx);sum+=a[t2];}
                for(int t2=0;t2<eff;t2++)a[t2]/=sum;
                float* o=ao+t1*C+h*HS;
                for(int d=0;d<HS;d++)o[d]=0;
                for(int t2=0;t2<eff;t2++){
                    float w=a[t2];const float* v2=v+t2*C+h*HS;
                    for(int d=0;d<HS;d++)o[d]+=w*v2[d];
                }
            }
            // Wo + res1
            const float* wo=m->p.wo+l*C*C;const float* bo=m->p.bo+l*C;
            // 先把ln1残差暂存到ln2
            memcpy(ln2,x,(size_t)eff*C*sizeof(float));
            for(int i=0;i<eff;i++){
                const float* ai=ao+i*C;float* yi=x+i*C;const float* res=ln2+i*C;
                for(int j=0;j<C;j++){
                    float s=bo[j];const float* wj=wo+j*C;
                    for(int kk=0;kk<C;kk++)s+=ai[kk]*wj[kk];
                    yi[j]=res[j]+s;
                }
            }
            // LN2
            for(int i=0;i<eff;i++){
                const float* xi=x+i*C;float* oi=ln1+i*C;
                float mm=0,vv=0;
                for(int j=0;j<C;j++)mm+=xi[j];mm/=C;
                for(int j=0;j<C;j++){float d=xi[j]-mm;vv+=d*d;}vv/=C;
                float iss=1.0f/sqrtf(vv+1e-5f);
                const float* g=m->p.ln2g+l*C;const float* b=m->p.ln2b+l*C;
                for(int j=0;j<C;j++){float h=(xi[j]-mm)*iss;oi[j]=g[j]*h+b[j];}
            }
            memcpy(ln2,x,(size_t)eff*C*sizeof(float)); // 保存ln2_in
            // FC + GELU
            const float* wfc=m->p.wfc+l*FC*C;const float* bfc=m->p.bfc+l*FC;
            for(int i=0;i<eff;i++){
                const float* xi=ln1+i*C;float* yi=fc+i*FC;
                for(int j=0;j<FC;j++){
                    float s=bfc[j];const float* wj=wfc+j*C;
                    for(int kk=0;kk<C;kk++)s+=xi[kk]*wj[kk];
                    float cc=sqrtf(2.0f/(float)M_PI);
                    yi[j]=0.5f*s*(1+tanhf(cc*(s+0.044715f*s*s*s)));
                }
            }
            // Proj + res2
            const float* wp=m->p.wproj+l*C*FC;const float* bp=m->p.bproj+l*C;
            for(int i=0;i<eff;i++){
                const float* fi=fc+i*FC;float* yi=x+i*C;const float* res=ln2+i*C;
                for(int j=0;j<C;j++){
                    float s=bp[j];const float* wj=wp+j*FC;
                    for(int kk=0;kk<FC;kk++)s+=fi[kk]*wj[kk];
                    yi[j]=res[j]+s;
                }
            }
        }
        // Final LN
        for(int i=0;i<eff;i++){
            const float* xi=x+i*C;float* oi=fin+i*C;
            float mm=0,vv=0;
            for(int j=0;j<C;j++)mm+=xi[j];mm/=C;
            for(int j=0;j<C;j++){float d=xi[j]-mm;vv+=d*d;}vv/=C;
            float iss=1.0f/sqrtf(vv+1e-5f);
            for(int j=0;j<C;j++){oi[j]=m->p.lnfg[j]*(xi[j]-mm)*iss+m->p.lnfb[j];}
        }
        if (pos >= plen-1 && pos < total-1){
            int last = eff-1;
            for(int j=0;j<V;j++){
                float s=0;const float* fi=fin+last*C;const float* wj=m->p.wte+j*C;
                for(int kk=0;kk<C;kk++)s+=fi[kk]*wj[kk];
                logits[j]=s/temp;
            }
            float mx=-1e9f,sum=0;float probs[256];
            for(int j=0;j<V;j++)if(logits[j]>mx)mx=logits[j];
            for(int j=0;j<V;j++){probs[j]=expf(logits[j]-mx);sum+=probs[j];}
            for(int j=0;j<V;j++)probs[j]/=sum;
            float r=cllm_rand_uniform(0,1),cu=0;int tok=0;
            for(int j=0;j<V;j++){cu+=probs[j];if(cu>=r){tok=j;break;}}
            out[pos+1]=tok;
        }
        if(eff==T){
            memmove(x, x+C, (size_t)(T-1)*C*sizeof(float));
        }
    }
}

// ===== Save/Load =====
int gpt_save(GPTModel *m, const char *path){
    FILE *fp=fopen(path,"wb");if(!fp)return -1;
    uint32_t magic=0x434C4C4D,ver=1;
    fwrite(&magic,4,1,fp);fwrite(&ver,4,1,fp);
    fwrite(&m->cfg,sizeof(GPTConfig),1,fp);
    int C=m->cfg.d_model,L=m->cfg.n_layers,F=4*C,V=m->cfg.vocab_size,T=m->cfg.block_size;
    fwrite(m->p.wte,sizeof(float),(size_t)V*C,fp);
    fwrite(m->p.wpe,sizeof(float),(size_t)T*C,fp);
    fwrite(m->p.ln1g,sizeof(float),(size_t)L*C,fp);fwrite(m->p.ln1b,sizeof(float),(size_t)L*C,fp);
    fwrite(m->p.wq,sizeof(float),(size_t)L*C*C,fp);fwrite(m->p.wk,sizeof(float),(size_t)L*C*C,fp);
    fwrite(m->p.wv,sizeof(float),(size_t)L*C*C,fp);fwrite(m->p.wo,sizeof(float),(size_t)L*C*C,fp);
    fwrite(m->p.bq,sizeof(float),(size_t)L*C,fp);fwrite(m->p.bk,sizeof(float),(size_t)L*C,fp);
    fwrite(m->p.bv,sizeof(float),(size_t)L*C,fp);fwrite(m->p.bo,sizeof(float),(size_t)L*C,fp);
    fwrite(m->p.ln2g,sizeof(float),(size_t)L*C,fp);fwrite(m->p.ln2b,sizeof(float),(size_t)L*C,fp);
    fwrite(m->p.wfc,sizeof(float),(size_t)L*F*C,fp);fwrite(m->p.bfc,sizeof(float),(size_t)L*F,fp);
    fwrite(m->p.wproj,sizeof(float),(size_t)L*C*F,fp);fwrite(m->p.bproj,sizeof(float),(size_t)L*C,fp);
    fwrite(m->p.lnfg,sizeof(float),C,fp);fwrite(m->p.lnfb,sizeof(float),C,fp);
    fclose(fp);return 0;
}
GPTModel* gpt_load(const char *path){
    FILE *fp=fopen(path,"rb");if(!fp)return NULL;
    uint32_t magic,ver;
    if(fread(&magic,4,1,fp)!=1||magic!=0x434C4C4D){fclose(fp);return NULL;}
    if(fread(&ver,4,1,fp)!=1||ver!=1){fclose(fp);return NULL;}
    GPTConfig cfg;
    if(fread(&cfg,sizeof(GPTConfig),1,fp)!=1){fclose(fp);return NULL;}
    GPTModel *m=gpt_create(&cfg);
    int C=cfg.d_model,L=cfg.n_layers,F=4*C,V=cfg.vocab_size,T=cfg.block_size;
    size_t n;
    #define RD(name,sz) if(fread(m->p.name,sizeof(float),(sz),fp)!=(sz)){gpt_free(m);fclose(fp);return NULL;}
    RD(wte,(size_t)V*C);RD(wpe,(size_t)T*C);
    RD(ln1g,(size_t)L*C);RD(ln1b,(size_t)L*C);
    RD(wq,(size_t)L*C*C);RD(wk,(size_t)L*C*C);RD(wv,(size_t)L*C*C);RD(wo,(size_t)L*C*C);
    RD(bq,(size_t)L*C);RD(bk,(size_t)L*C);RD(bv,(size_t)L*C);RD(bo,(size_t)L*C);
    RD(ln2g,(size_t)L*C);RD(ln2b,(size_t)L*C);
    RD(wfc,(size_t)L*F*C);RD(bfc,(size_t)L*F);RD(wproj,(size_t)L*C*F);RD(bproj,(size_t)L*C);
    RD(lnfg,C);RD(lnfb,C);
    #undef RD
    (void)n;
    fclose(fp);return m;
}
