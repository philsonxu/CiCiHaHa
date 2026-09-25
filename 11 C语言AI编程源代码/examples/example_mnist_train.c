/**
 * @file example_mnist_train.c - MNIST手写数字识别训练程序
 *
 * 使用方法:
 *   1. 下载MNIST数据集 (4个IDX文件), 放到data/mnist/下:
 *      - train-images-idx3-ubyte
 *      - train-labels-idx1-ubyte
 *      - t10k-images-idx3-ubyte
 *      - t10k-labels-idx1-ubyte
 *   2. 编译: make
 *   3. 运行: ./example_mnist_train
 *
 * 支持两种模型: --mlp (MLP) 或 --cnn (LeNet风格CNN)
 * 默认使用MLP快速训练, CNN需要更多时间
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "aimath.h"
#include "dataset.h"
#include "neuralnet.h"
#include "cnn.h"

int main(int argc, char** argv) {
    int use_cnn = 0;
    int epochs = 20;
    int max_train = 0; /* 0 = all 60000 */
    int max_test = 0;
    for (int i=1;i<argc;i++) {
        if (strcmp(argv[i],"--cnn")==0) use_cnn=1;
        else if (strcmp(argv[i],"--epochs")==0) epochs=atoi(argv[++i]);
        else if (strcmp(argv[i],"--max-train")==0) max_train=atoi(argv[++i]);
        else if (strcmp(argv[i],"--max-test")==0) max_test=atoi(argv[++i]);
    }

    printf("========== MNIST手写数字识别 ==========\n");
    printf("加载MNIST数据集...\n");
    Dataset* train = dataset_load_mnist("data/mnist/train-images-idx3-ubyte",
                                         "data/mnist/train-labels-idx1-ubyte", max_train);
    Dataset* test = dataset_load_mnist("data/mnist/t10k-images-idx3-ubyte",
                                        "data/mnist/t10k-labels-idx1-ubyte", max_test);
    if (!train || !test) {
        printf("MNIST数据未找到! 请将4个IDX文件放在 data/mnist/ 目录下。\n");
        printf("可从 http://yann.lecun.com/exdb/mnist/ 下载\n");
        printf("\n若没有MNIST数据, 可以运行其他example程序: test_classic_ml / test_nn / test_cnn\n");
        if (train) dataset_free(train);
        if (test) dataset_free(test);
        return 1;
    }
    printf("训练集: %zu张, 测试集: %zu张\n", train->n, test->n);

    random_seed(42);

    if (use_cnn) {
        printf("\n--- 使用CNN模型 (Conv-ReLU-Pool-Conv-ReLU-Pool-FC128-FC10) ---\n");
        CNN* net = cnn_create();
        cnn_set_adam(net, 0.001f, 0.9f, 0.999f, 1e-8f, 1e-4f);
        cnn_set_verbose(net, 1);
        cnn_fit(net, train, test, epochs);
        float acc = cnn_evaluate(net, test, 0);
        printf("\n最终测试集准确率: %.2f%%\n", acc*100);
        cnn_save(net, "mnist_cnn_model.bin");
        printf("模型已保存到 mnist_cnn_model.bin\n");
        cnn_free(net);
    } else {
        printf("\n--- 使用MLP模型 (784-256-128-10, ReLU+Adam) ---\n");
        size_t dims[] = {784, 256, 128, 10};
        Activation acts[] = {ACT_RELU, ACT_RELU, ACT_SOFTMAX};
        NeuralNet* net = nn_create(dims, acts, 4);
        nn_set_optimizer_adam(net, 0.001f, 0.9f, 0.999f, 1e-8f, 1e-4f);
        nn_set_batch_size(net, 128);
        nn_set_verbose(net, 1);
        nn_fit(net, train, test, epochs);
        float loss, acc;
        nn_evaluate(net, test, &loss, &acc);
        printf("\n最终测试集 loss=%.4f, 准确率=%.2f%%\n", loss, acc*100);
        nn_save(net, "mnist_mlp_model.bin");
        printf("模型已保存到 mnist_mlp_model.bin\n");
        nn_free(net);
    }
    dataset_free(train); dataset_free(test);
    return 0;
}
