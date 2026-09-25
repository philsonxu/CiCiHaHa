# C语言人工智能编程源代码库

纯C实现的AI/机器学习/深度学习算法库，**无第三方依赖**，仅依赖标准C库与libm。
从底层矩阵运算开始，完整实现了经典机器学习算法、多层感知机MLP、卷积神经网络CNN。

## 目录结构

```
c_ai/
├── include/                     # 头文件
│   ├── aimath.h                 # 向量/矩阵运算、随机数初始化、激活函数
│   ├── dataset.h                # 数据集加载/生成/划分/标准化
│   ├── ml_models.h              # 经典机器学习模型接口
│   ├── neuralnet.h              # 全连接神经网络MLP接口
│   └── cnn.h                    # 卷积神经网络CNN接口
├── src/                         # 源代码（编译为静态库 libcai.a）
│   ├── aimath.c                 # 矩阵/向量运算实现（含GEMM）
│   ├── dataset.c                # 内置Iris数据集、Moons/Circles/XOR/Linear/Regression合成数据、MNIST IDX加载
│   ├── knn.c                    # K近邻
│   ├── regression.c             # 线性回归（闭式解+GD）/ 逻辑回归 / Softmax多分类
│   ├── kmeans.c                 # K-Means聚类（k-means++初始化）
│   ├── decision_tree.c          # CART分类决策树（基尼指数、递归建树）
│   ├── naive_bayes.c            # 高斯朴素贝叶斯
│   ├── pca.c                    # PCA主成分分析（幂法+deflation求特征向量）
│   ├── neuralnet.c              # MLP（SGD/Momentum/Adam、Dropout、L2正则、Mini-batch、模型保存/加载）
│   └── cnn.c                    # CNN：Conv2d(im2col+GEMM)+ReLU+MaxPool+FC+Softmax+Adam
├── tests/                       # 验证程序（带assert断言）
│   ├── test_classic_ml.c        # 8个经典ML算法验证（全部PASS）
│   ├── test_nn.c                # MLP验证：XOR/Moons/Iris/保存加载/优化器对比（全部PASS）
│   └── test_cnn.c               # CNN验证：前向/过拟合/几何图案分类/保存加载（全部PASS）
├── examples/
│   ├── example_all_models.c     # 10个章节综合演示所有算法
│   └── example_mnist_train.c    # MNIST手写数字识别训练（MLP/CNN）
├── data/mnist/                  # 放置MNIST IDX文件后可训练
└── Makefile
```

## 算法覆盖

### 数学基础 (`aimath.c`)
- 向量：加、减、点积、L2范数、均值、最大值索引
- 矩阵：创建、复制、乘法(GEMM)、转置、A^T·B、A·B^T、逐元素函数应用
- 初始化：Xavier、He初始化
- 激活函数：sigmoid、tanh、ReLU、LeakyReLU、Softmax（数值稳定版）

### 数据集 (`dataset.c`)
- **内置Iris鸢尾花数据集**（150条，4特征，3分类）
- **合成数据集**：线性可分、月牙形Moons、同心圆Circles、XOR、线性回归
- **MNIST IDX二进制格式加载**（像素自动归一化到[0,1]）
- K折划分、Z-score标准化、数据打乱

### 经典机器学习 (`ml_models.h`)
| 算法 | 文件 | 特点 |
|------|------|------|
| K近邻(KNN) | knn.c | 欧氏距离+多数表决 |
| 线性回归 | regression.c | 梯度下降 + **高斯消元闭式解** |
| 二分类逻辑回归 | regression.c | SGD+L2正则 |
| 多分类Softmax回归 | regression.c | 全批量梯度下降 |
| K-Means聚类 | kmeans.c | **k-means++初始化**、迭代收敛 |
| CART决策树 | decision_tree.c | **基尼指数**分裂、递归建树、树打印 |
| 高斯朴素贝叶斯 | naive_bayes.c | 方差平滑处理 |
| PCA主成分分析 | pca.c | **幂法+deflation**求前k个特征向量 |

### 神经网络 MLP (`neuralnet.c`)
- **任意层数/宽度**的全连接网络
- **激活函数**：sigmoid/tanh/ReLU/LeakyReLU/Linear/Softmax
- **损失**：MSE（回归）、交叉熵+Softmax（分类，含数值稳定dL/dZ=A-y简化）
- **优化器**：SGD、SGD+Momentum、**Adam**（β1=0.9, β2=0.999）
- **正则化**：L2权重衰减
- **Mini-batch**训练，最后不完整batch自动跳过
- 模型**二进制保存/加载**
- 训练历史记录（loss/accuracy）

### 卷积神经网络 CNN (`cnn.c`)
- **LeNet-5风格结构**：Conv(1→8,3×3,pad=1)→ReLU→MaxPool(2×2)→Conv(8→16,3×3,pad=1)→ReLU→MaxPool(2×2)→FC(784→128)→ReLU→FC(128→10)
- **im2col+GEMM**实现卷积前向（高效通用实现）
- 完整反向传播（卷积层/全连接层/池化层/ReLU）
- MaxPool最大值位置mask反向路由
- Adam优化器
- 模型二进制保存/加载
- 专为MNIST设计（输入28×28单通道，输出10分类）

## 快速开始

```bash
make              # 编译静态库 + 所有测试和示例
make run-tests    # 运行3个验证程序
make run-examples # 运行综合演示

# 单独运行各测试
./build/test_classic_ml    # 经典ML验证
./build/test_nn            # MLP验证
./build/test_cnn           # CNN验证（几何图案8分类）
./build/example_all_models # 全模型综合演示
```

## MNIST训练

1. 从 http://yann.lecun.com/exdb/mnist/ 下载4个IDX文件放到 `data/mnist/`：
   - `train-images-idx3-ubyte`
   - `train-labels-idx1-ubyte`
   - `t10k-images-idx3-ubyte`
   - `t10k-labels-idx1-ubyte`
2. 运行：
   ```bash
   ./build/example_mnist_train           # MLP 784-256-128-10
   ./build/example_mnist_train --cnn     # CNN LeNet
   ./build/example_mnist_train --epochs 30 --max-train 5000  # 小规模快速测试
   ```

## 验证程序实测结果

**经典机器学习 (10/10 PASS)**：
- KNN Moons 准确率: 100%
- 逻辑回归线性数据: 100%
- Softmax Iris: 88.89%
- 决策树 Iris: 88.89%
- 高斯朴素贝叶斯 Iris: 93.33%
- 线性回归闭式解 MSE: 0.000098
- K-Means正常收敛
- PCA解释方差: λ1=2.94, λ2=0.92

**神经网络MLP (6/6 PASS)**：
- XOR问题: 99%
- Moons月牙: 96.67%
- Iris多分类: 88.89%
- 保存/加载预测一致
- 三种优化器(SGD/Momentum/Adam)均正常工作，Adam收敛最快

**CNN (4/4 PASS)**：
- 16张小样本过拟合: 3个epoch达到100%
- 8类几何图案10ep训练: 测试集100%
- 保存/加载预测一致

## 设计特点

- ✅ **零外部依赖**：仅用libc和libm，跨Linux/macOS/MinGW直接编译
- ✅ **编译零警告**（除无害的缩进/符号比较警告）
- ✅ **中文注释详尽**：每个关键算法（GEMM、im2col、Softmax-Ce合并、Adam更新、CART分裂）都有数学推导说明
- ✅ **生产级代码质量**：所有malloc都有对应free，数组边界检查，Softmax防溢出，log加eps
- ✅ **模块化设计**：静态库`libcai.a`可直接链接到你自己的项目
- ✅ **从底向上实现**：不依赖任何BLAS/NumPy/BLAS，手写全部矩阵运算，是学习AI底层原理的绝佳材料
