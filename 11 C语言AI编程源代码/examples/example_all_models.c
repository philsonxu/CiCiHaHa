/**
 * @file example_all_models.c - 所有AI模型综合使用示例
 *
 * 运行后依次演示:
 *   1. 线性回归拟合
 *   2. 逻辑回归二分类
 *   3. KNN分类
 *   4. 决策树 + 树结构打印
 *   5. 朴素贝叶斯
 *   6. Softmax多分类 (Iris)
 *   7. K-Means聚类
 *   8. PCA降维可视化数据生成
 *   9. MLP神经网络分类/回归
 *   10. CNN几何图案识别
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "aimath.h"
#include "dataset.h"
#include "ml_models.h"
#include "neuralnet.h"
#include "cnn.h"

/* 图案生成函数 (与test_cnn中相同) */
extern void gen_pattern(float* img, int cls, float noise); /* 声明 */
static void _gen_pattern(float* img, int cls, float noise) {
    memset(img, 0, 784*sizeof(float));
    int cx=14+(int)random_uniform(-3,3), cy=14+(int)random_uniform(-3,3);
    switch(cls) {
        case 0: for(int x=cx-8;x<=cx+8;x++) if(x>=0&&x<28)img[cy*28+x]=1;break;
        case 1: for(int y=cy-8;y<=cy+8;y++) if(y>=0&&y<28)img[y*28+cx]=1;break;
        case 2: for(int d=-8;d<=8;d++){int y=cy+d,x=cx+d;if(y>=0&&y<28&&x>=0&&x<28)img[y*28+x]=1;}break;
        case 3: for(int d=-8;d<=8;d++){int y=cy+d,x=cx-d;if(y>=0&&y<28&&x>=0&&x<28)img[y*28+x]=1;}break;
        case 4: for(int x=cx-7;x<=cx+7;x++)if(x>=0&&x<28)img[cy*28+x]=1;for(int y=cy-7;y<=cy+7;y++)if(y>=0&&y<28)img[y*28+cx]=1;break;
        case 5: for(int y=cy-5;y<=cy+5;y++)for(int x=cx-5;x<=cx+5;x++)if(y>=0&&y<28&&x>=0&&x<28)img[y*28+x]=1;break;
        case 6: for(int y=cy-6;y<=cy+6;y++)for(int x=cx-6;x<=cx+6;x++)if((x-cx)*(x-cx)+(y-cy)*(y-cy)<=36&&y>=0&&y<28&&x>=0&&x<28)img[y*28+x]=1;break;
        case 7: for(int x=cx-5;x<=cx+5;x++){if(cy-5>=0)img[(cy-5)*28+x]=1;if(cy+5<28)img[(cy+5)*28+x]=1;}
                for(int y=cy-5;y<=cy+5;y++){if(cx-5>=0)img[y*28+cx-5]=1;if(cx+5<28)img[y*28+cx+5]=1;}break;
    }
    for(int i=0;i<784;i++){img[i]+=random_normal(0,noise);if(img[i]<0)img[i]=0;if(img[i]>1)img[i]=1;}
}

static Dataset* make_shapes(int npc, float noise) {
    int n=npc*8; Dataset* ds=dataset_create(n,784,8);
    for(int c=0;c<8;c++)for(int i=0;i<npc;i++){int idx=c*npc+i;
        _gen_pattern((float*)mat_at(&ds->X,idx,0),c,noise);
        ds->labels[idx]=c; for(int k=0;k<8;k++)*mat_at(&ds->y,idx,k)=(k==c);}
    return ds;
}

void section(const char* title) {
    printf("\n------------------------------------------------\n");
    printf("  %s\n", title);
    printf("------------------------------------------------\n");
}

int main(void) {
    random_seed(42);
    printf("╔══════════════════════════════════════════════╗\n");
    printf("║   C语言AI编程 - 全部模型综合演示程序         ║\n");
    printf("╚══════════════════════════════════════════════╝\n");

    /* ===== 1. 线性回归 ===== */
    section("1. 线性回归 (闭式解)");
    {
        Dataset* ds = dataset_make_regression(200, 2, 0.05f);
        LinearRegression* model = lr_create(2, 0.01f, 100);
        lr_train_closed(model, ds);
        float* pred = (float*)malloc(ds->n*sizeof(float));
        lr_predict(model, ds, pred);
        Matrix P={ds->n,1,pred};
        printf("  学习到的参数: w=[%.3f, %.3f], b=%.3f\n", model->w[0], model->w[1], model->b);
        printf("  MSE = %.6f\n", mse_score(&P, &ds->y));
        free(pred); lr_free(model); dataset_free(ds);
    }

    /* ===== 2. 逻辑回归 ===== */
    section("2. 逻辑回归 (线性可分二分类)");
    {
        Dataset* ds = dataset_make_linear(300);
        Dataset *test; Dataset* train = dataset_split(ds, 0.7f, &test);
        LogisticRegression* m = logreg_create(2, 0.1f, 1e-4f, 500);
        logreg_train(m, train);
        int* pred=(int*)malloc(test->n*sizeof(int));
        logreg_predict(m,test,pred);
        printf("  准确率: %.2f%%\n", accuracy_score_labels(pred,test->labels,test->n)*100);
        free(pred); logreg_free(m);
        dataset_free(train); dataset_free(test); dataset_free(ds);
    }

    /* ===== 3. KNN ===== */
    section("3. K近邻 (Moons月牙)");
    {
        Dataset* ds = dataset_make_moons(300, 0.1f);
        Dataset *test; Dataset* train = dataset_split(ds, 0.7f, &test);
        KNN* m = knn_create(7); knn_train(m,train);
        int* pred=(int*)malloc(test->n*sizeof(int));
        knn_predict(m,test,pred);
        printf("  k=7 准确率: %.2f%%\n", accuracy_score_labels(pred,test->labels,test->n)*100);
        free(pred); knn_free(m);
        dataset_free(train);dataset_free(test);dataset_free(ds);
    }

    /* ===== 4. 决策树 ===== */
    section("4. CART决策树 (Iris, 打印树结构)");
    {
        Dataset* ds = dataset_load_iris();
        Dataset *test; Dataset* train = dataset_split(ds, 0.7f, &test);
        DecisionTree* m = dt_create(4, 3);
        dt_train(m, train);
        int* pred=(int*)malloc(test->n*sizeof(int));
        dt_predict(m,test,pred);
        printf("  准确率: %.2f%%\n", accuracy_score_labels(pred,test->labels,test->n)*100);
        printf("  树结构:\n"); dt_print(m);
        free(pred); dt_free(m);
        dataset_free(train);dataset_free(test);dataset_free(ds);
    }

    /* ===== 5. 朴素贝叶斯 ===== */
    section("5. 高斯朴素贝叶斯 (Iris)");
    {
        Dataset* ds = dataset_load_iris();
        Dataset *test; Dataset* train = dataset_split(ds, 0.7f, &test);
        GaussianNB* m = gnb_create(4,3,1e-9f); gnb_train(m,train);
        int* pred=(int*)malloc(test->n*sizeof(int));
        gnb_predict(m,test,pred);
        printf("  准确率: %.2f%%\n", accuracy_score_labels(pred,test->labels,test->n)*100);
        free(pred); gnb_free(m);
        dataset_free(train);dataset_free(test);dataset_free(ds);
    }

    /* ===== 6. Softmax回归 ===== */
    section("6. Softmax多分类 (Iris)");
    {
        Dataset* ds = dataset_load_iris();
        Dataset *test; Dataset* train = dataset_split(ds, 0.7f, &test);
        SoftmaxRegression* m = sm_create(4,3,0.1f,1e-4f,800);
        sm_train(m,train);
        int* pred=(int*)malloc(test->n*sizeof(int));
        sm_predict(m,test,pred);
        printf("  准确率: %.2f%%\n", accuracy_score_labels(pred,test->labels,test->n)*100);
        /* 查看权重矩阵第一列 */
        printf("  第一类特征权重: [%.2f, %.2f, %.2f, %.2f]\n",
               *mat_at_c(&m->W,0,0), *mat_at_c(&m->W,1,0), *mat_at_c(&m->W,2,0), *mat_at_c(&m->W,3,0));
        free(pred); sm_free(m);
        dataset_free(train);dataset_free(test);dataset_free(ds);
    }

    /* ===== 7. KMeans ===== */
    section("7. K-Means聚类 (月牙形)");
    {
        Dataset* ds = dataset_make_moons(400, 0.05f);
        KMeans* km = kmeans_create(2, 2, 100, 1e-6f);
        float inertia; int it = kmeans_fit(km, ds, &inertia);
        printf("  迭代%d次, inertia=%.2f\n", it, inertia);
        printf("  聚类中心: (%.2f,%.2f), (%.2f,%.2f)\n",
               *mat_at_c(&km->centroids,0,0), *mat_at_c(&km->centroids,0,1),
               *mat_at_c(&km->centroids,1,0), *mat_at_c(&km->centroids,1,1));
        printf("  (注: K-Means对非凸簇月牙形效果有限, 此处仅演示)\n");
        kmeans_free(km); dataset_free(ds);
    }

    /* ===== 8. PCA ===== */
    section("8. PCA降维 (Iris 4D->2D)");
    {
        Dataset* ds = dataset_load_iris();
        PCA* pca = pca_create(2); pca_fit(pca, ds);
        printf("  主成分1解释方差: %.3f\n", pca->explained_var[0]);
        printf("  主成分2解释方差: %.3f\n", pca->explained_var[1]);
        printf("  累计解释方差比: %.1f%%\n",
               (pca->explained_var[0]+pca->explained_var[1])/
               (pca->explained_var[0]+pca->explained_var[1]+pca->explained_var[2]+pca->explained_var[3])*100);
        pca_free(pca); dataset_free(ds);
    }

    /* ===== 9. MLP ===== */
    section("9. MLP神经网络 (XOR + Moons)");
    {
        /* XOR */
        Dataset* ds = dataset_make_xor(200);
        size_t dims[]={2,8,2}; Activation acts[]={ACT_TANH, ACT_SOFTMAX};
        NeuralNet* net = nn_create(dims, acts, 3);
        nn_set_optimizer_adam(net, 0.01f, 0.9f, 0.999f, 1e-8f, 0);
        nn_set_verbose(net, 0);
        nn_fit(net, ds, NULL, 200);
        int* pred=(int*)malloc(ds->n*sizeof(int));
        nn_predict(net,ds,pred);
        printf("  XOR问题准确率: %.2f%%\n", accuracy_score_labels(pred,ds->labels,ds->n)*100);
        free(pred); nn_free(net); dataset_free(ds);

        /* Moons */
        ds = dataset_make_moons(400, 0.2f);
        Dataset *test; Dataset* train = dataset_split(ds,0.7f,&test);
        size_t dims2[]={2,16,16,2}; Activation acts2[]={ACT_RELU,ACT_RELU,ACT_SOFTMAX};
        net = nn_create(dims2, acts2, 4);
        nn_set_optimizer_adam(net,0.005f,0.9f,0.999f,1e-8f,1e-5f);
        nn_set_batch_size(net,32); nn_set_verbose(net,0);
        nn_fit(net, train, test, 200);
        pred=(int*)malloc(test->n*sizeof(int));
        nn_predict(net,test,pred);
        printf("  Moons MLP准确率: %.2f%%\n", accuracy_score_labels(pred,test->labels,test->n)*100);
        /* 保存模型 */
        nn_save(net, "moons_mlp_model.bin");
        printf("  模型已保存到 moons_mlp_model.bin\n");
        free(pred); nn_free(net);
        dataset_free(train);dataset_free(test);dataset_free(ds);
    }

    /* ===== 10. CNN ===== */
    section("10. CNN卷积神经网络 (几何图案8分类)");
    {
        Dataset* train = make_shapes(30, 0.1f);
        Dataset* test = make_shapes(10, 0.1f);
        CNN* net = cnn_create();
        cnn_set_adam(net, 0.002f, 0.9f, 0.999f, 1e-8f, 1e-5f);
        cnn_set_verbose(net, 0);
        cnn_fit(net, train, test, 5);
        float acc = cnn_evaluate(net, test, 0);
        printf("  8类几何图案 CNN准确率: %.2f%%\n", acc*100);
        cnn_save(net, "shapes_cnn_model.bin");
        printf("  模型已保存到 shapes_cnn_model.bin\n");
        cnn_free(net);
        dataset_free(train); dataset_free(test);
    }

    printf("\n╔══════════════════════════════════════════════╗\n");
    printf("║           全部模型演示完成!                  ║\n");
    printf("╚══════════════════════════════════════════════╝\n");
    return 0;
}
