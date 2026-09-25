/**
 * @file test_cnn.c - CNN卷积神经网络验证程序
 * 使用合成28x28几何图案 (8类) 验证CNN训练能力
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "aimath.h"
#include "dataset.h"
#include "cnn.h"

static int passed=0, total=0;
#define CHECK(name, cond) do {total++; if(cond){passed++;printf("  [PASS] %s\n",name);} else{printf("  [FAIL] %s\n",name);}}while(0)

static void _gen_pattern(float* img, int cls, float noise) {
    memset(img, 0, 784*sizeof(float));
    int cx=14+(int)random_uniform(-3,3), cy=14+(int)random_uniform(-3,3);
    switch(cls) {
        case 0: for(int x=cx-8;x<=cx+8;x++) if(x>=0&&x<28)img[cy*28+x]=1;break;
        case 1: for(int y=cy-8;y<=cy+8;y++) if(y>=0&&y<28)img[y*28+cx]=1;break;
        case 2: for(int d=-8;d<=8;d++){int y=cy+d,x=cx+d;if(y>=0&&y<28&&x>=0&&x<28)img[y*28+x]=1;}break;
        case 3: for(int d=-8;d<=8;d++){int y=cy+d,x=cx-d;if(y>=0&&y<28&&x>=0&&x<28)img[y*28+x]=1;}break;
        case 4: for(int x=cx-7;x<=cx+7;x++)if(x>=0&&x<28)img[cy*28+x]=1;
                for(int y=cy-7;y<=cy+7;y++)if(y>=0&&y<28)img[y*28+cx]=1;break;
        case 5: for(int y=cy-5;y<=cy+5;y++)for(int x=cx-5;x<=cx+5;x++)
                if(y>=0&&y<28&&x>=0&&x<28)img[y*28+x]=1;break;
        case 6: for(int y=cy-6;y<=cy+6;y++)for(int x=cx-6;x<=cx+6;x++)
                if((x-cx)*(x-cx)+(y-cy)*(y-cy)<=36&&y>=0&&y<28&&x>=0&&x<28)img[y*28+x]=1;break;
        case 7: for(int x=cx-5;x<=cx+5;x++){if(cy-5>=0)img[(cy-5)*28+x]=1;if(cy+5<28)img[(cy+5)*28+x]=1;}
                for(int y=cy-5;y<=cy+5;y++){if(cx-5>=0)img[y*28+(cx-5)]=1;if(cx+5<28)img[y*28+(cx+5)]=1;}break;
    }
    for(int i=0;i<784;i++){
        img[i]+=random_normal(0,noise);
        if(img[i]<0) img[i]=0;
        if(img[i]>1) img[i]=1;
    }
}

static Dataset* make_shapes(int npc, float noise) {
    int n=npc*8; Dataset* ds=dataset_create(n,784,8);
    for(int c=0;c<8;c++)for(int i=0;i<npc;i++){int idx=c*npc+i;
        _gen_pattern((float*)mat_at(&ds->X,idx,0),c,noise);
        ds->labels[idx]=c;
        for(int k=0;k<8;k++) *mat_at(&ds->y,idx,k)=(k==c)?1:0;}
    return ds;
}

void test_cnn_predict(void) {
    printf("\n=== CNN前向/预测测试 ===\n");
    random_seed(42);
    CNN* net = cnn_create();
    float img[784];
    _gen_pattern(img, 3, 0.0f);
    int pred = cnn_predict_one(net, img);
    printf("  随机初始化网络预测类别: %d\n", pred);
    CHECK("predict output in [0,9]", pred >= 0 && pred < 10);
    cnn_free(net);
}

void test_cnn_overfit(void) {
    printf("\n=== CNN小样本过拟合测试 (16张图记忆) ===\n");
    random_seed(42);
    Dataset* ds = make_shapes(2, 0.05f);
    CNN* net = cnn_create();
    cnn_set_adam(net, 0.003f, 0.9f, 0.999f, 1e-8f, 1e-5f);
    cnn_set_verbose(net, 1);
    cnn_fit(net, ds, NULL, 30);
    float acc = cnn_evaluate(net, ds, 0);
    printf("  16张图训练集准确率: %.2f%%\n", acc*100);
    CHECK("CNN overfit small dataset acc >= 80%", acc >= 0.80f);
    cnn_free(net); dataset_free(ds);
}

void test_cnn_shapes(void) {
    printf("\n=== CNN几何图案8分类测试 ===\n");
    random_seed(42);
    Dataset* train = make_shapes(50, 0.15f);
    Dataset* test = make_shapes(20, 0.15f);
    CNN* net = cnn_create();
    cnn_set_adam(net, 0.002f, 0.9f, 0.999f, 1e-8f, 1e-5f);
    cnn_set_verbose(net, 1);
    cnn_fit(net, train, test, 10);
    float test_acc = cnn_evaluate(net, test, 0);
    printf("  8类几何图案测试集准确率: %.2f%%\n", test_acc*100);
    CHECK("CNN shapes test accuracy > 80%", test_acc > 0.80f);
    cnn_free(net);
    dataset_free(train); dataset_free(test);
}

void test_cnn_save_load(void) {
    printf("\n=== CNN模型保存/加载测试 ===\n");
    random_seed(42);
    CNN* net = cnn_create();
    float img[784]; _gen_pattern(img, 5, 0.0f);
    int p1 = cnn_predict_one(net, img);
    cnn_save(net, "/tmp/test_cnn_model.bin");
    CNN* net2 = cnn_load("/tmp/test_cnn_model.bin");
    int p2 = cnn_predict_one(net2, img);
    printf("  保存前预测=%d, 加载后预测=%d\n", p1, p2);
    CHECK("CNN save/load consistent", p1 == p2);
    cnn_free(net); cnn_free(net2);
}

int main(void) {
    printf("========== CNN卷积神经网络验证程序 ==========\n");
    random_seed(42);
    test_cnn_predict();
    test_cnn_overfit();
    test_cnn_shapes();
    test_cnn_save_load();
    printf("\n========== 验证结果: %d/%d PASSED ==========\n", passed, total);
    return passed == total ? 0 : 1;
}
