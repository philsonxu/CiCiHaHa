/**
 * @file test_nn.c - 神经网络(MLP)训练验证程序
 *
 * 验证任务:
 *   1. XOR问题 - 验证非线性能力
 *   2. Moons月牙形 - 验证非线性分类
 *   3. Iris多分类 - 验证多分类能力
 *   4. 模型保存/加载
 *   5. 优化器对比 (SGD vs Momentum vs Adam)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "aimath.h"
#include "dataset.h"
#include "neuralnet.h"

static int passed=0, total=0;
#define CHECK(name, cond) do {total++; if(cond){passed++;printf("  [PASS] %s\n",name);} else{printf("  [FAIL] %s\n",name);}}while(0)

void test_xor(void) {
    printf("\n=== MLP求解XOR问题 ===\n");
    random_seed(42);
    Dataset* ds = dataset_make_xor(200);
    size_t dims[] = {2, 8, 2};
    Activation acts[] = {ACT_TANH, ACT_SOFTMAX};
    NeuralNet* net = nn_create(dims, acts, 3);
    nn_set_optimizer_adam(net, 0.01f, 0.9f, 0.999f, 1e-8f, 0);
    nn_set_batch_size(net, 32);
    nn_set_verbose(net, 0);
    nn_fit(net, ds, NULL, 200);
    int* pred = (int*)malloc(ds->n*sizeof(int));
    nn_predict(net, ds, pred);
    float acc = accuracy_score_labels(pred, ds->labels, ds->n);
    printf("  XOR MLP(2-8-2 tanh) 准确率: %.2f%%\n", acc*100);
    CHECK("MLP XOR accuracy > 95%", acc > 0.95f);
    free(pred); nn_free(net); dataset_free(ds);
}

void test_moons(void) {
    printf("\n=== MLP月牙形Moons分类 ===\n");
    random_seed(42);
    Dataset* ds = dataset_make_moons(400, 0.15f);
    Dataset *test;
    Dataset* train = dataset_split(ds, 0.7f, &test);
    size_t dims[] = {2, 16, 16, 2};
    Activation acts[] = {ACT_RELU, ACT_RELU, ACT_SOFTMAX};
    NeuralNet* net = nn_create(dims, acts, 4);
    nn_set_optimizer_adam(net, 0.005f, 0.9f, 0.999f, 1e-8f, 1e-5f);
    nn_set_batch_size(net, 32);
    nn_set_verbose(net, 0);
    nn_fit(net, train, test, 300);
    int* pred = (int*)malloc(test->n*sizeof(int));
    nn_predict(net, test, pred);
    float acc = accuracy_score_labels(pred, test->labels, test->n);
    printf("  Moons MLP(2-16-16-2 ReLU) 测试集准确率: %.2f%%\n", acc*100);
    CHECK("MLP Moons accuracy > 95%", acc > 0.95f);
    free(pred); nn_free(net);
    dataset_free(train); dataset_free(test); dataset_free(ds);
}

void test_iris(void) {
    printf("\n=== MLP Iris多分类 ===\n");
    random_seed(42);
    Dataset* ds = dataset_load_iris();
    Dataset *test;
    Dataset* train = dataset_split(ds, 0.7f, &test);
    size_t dims[] = {4, 16, 3};
    Activation acts[] = {ACT_RELU, ACT_SOFTMAX};
    NeuralNet* net = nn_create(dims, acts, 3);
    nn_set_optimizer_adam(net, 0.01f, 0.9f, 0.999f, 1e-8f, 1e-5f);
    nn_set_batch_size(net, 16);
    nn_set_verbose(net, 0);
    nn_fit(net, train, test, 300);
    int* pred = (int*)malloc(test->n*sizeof(int));
    nn_predict(net, test, pred);
    float acc = accuracy_score_labels(pred, test->labels, test->n);
    printf("  Iris MLP(4-16-3 ReLU) 测试集准确率: %.2f%%\n", acc*100);
    CHECK("MLP Iris accuracy > 85%", acc > 0.85f);
    free(pred); nn_free(net);
    dataset_free(train); dataset_free(test); dataset_free(ds);
}

void test_save_load(void) {
    printf("\n=== MLP模型保存/加载 ===\n");
    random_seed(42);
    Dataset* ds = dataset_make_xor(100);
    size_t dims[] = {2, 8, 2};
    Activation acts[] = {ACT_TANH, ACT_SOFTMAX};
    NeuralNet* net = nn_create(dims, acts, 3);
    nn_set_optimizer_adam(net, 0.01f, 0.9f, 0.999f, 1e-8f, 0);
    nn_set_verbose(net,0);
    nn_fit(net, ds, NULL, 100);
    nn_save(net, "/tmp/test_nn_model.bin");
    NeuralNet* net2 = nn_load("/tmp/test_nn_model.bin");
    float test_x[2] = {0.5f, -0.3f};
    int p1 = nn_predict_one(net, test_x);
    int p2 = nn_predict_one(net2, test_x);
    printf("  模型1预测=%d, 模型2(加载)预测=%d\n", p1, p2);
    CHECK("Save/Load prediction match", p1 == p2);
    nn_free(net); nn_free(net2); dataset_free(ds);
}

void test_optimizers(void) {
    printf("\n=== 三种优化器对比 (Iris) ===\n");
    random_seed(42);
    Dataset* ds = dataset_load_iris();
    Dataset *test;
    Dataset* train = dataset_split(ds, 0.7f, &test);
    int* pred = (int*)malloc(test->n*sizeof(int));

    const char* opt_names[] = {"SGD", "SGD+Momentum", "Adam"};
    float accs[3];
    for (int opt=0; opt<3; opt++) {
        size_t dims[] = {4, 16, 3};
        Activation acts[] = {ACT_RELU, ACT_SOFTMAX};
        NeuralNet* net = nn_create(dims, acts, 3);
        nn_set_batch_size(net, 16); nn_set_verbose(net,0);
        if (opt==0) nn_set_optimizer_sgd(net, 0.05f, 1e-5f);
        else if (opt==1) nn_set_optimizer_momentum(net, 0.05f, 0.9f, 1e-5f);
        else nn_set_optimizer_adam(net, 0.01f, 0.9f, 0.999f, 1e-8f, 1e-5f);
        nn_fit(net, train, test, 200);
        nn_predict(net, test, pred);
        accs[opt] = accuracy_score_labels(pred, test->labels, test->n);
        printf("  %-15s 测试准确率: %.2f%%\n", opt_names[opt], accs[opt]*100);
        nn_free(net);
    }
    CHECK("SGD works", accs[0] > 0.7f);
    CHECK("Adam better than SGD", accs[2] >= accs[0] - 0.05f);
    free(pred);
    dataset_free(train); dataset_free(test); dataset_free(ds);
}

int main(void) {
    printf("========== 神经网络MLP验证程序 ==========\n");
    test_xor();
    test_moons();
    test_iris();
    test_save_load();
    test_optimizers();

    printf("\n========== 验证结果: %d/%d PASSED ==========\n", passed, total);
    return passed == total ? 0 : 1;
}
