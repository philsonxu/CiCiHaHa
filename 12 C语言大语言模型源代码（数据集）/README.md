# C-LLM: 纯C语言大语言模型（GPT-2/Transformer）训练与推理库

从零实现的纯C Transformer语言模型，无任何第三方依赖（仅使用C标准库+libm）。支持：
- **完整的Decoder-only Transformer架构**：Token/Position Embedding → N×(Masked Multi-Head Self-Attention + MLP(GELU) + Pre-LN残差) → Final LN → LM Head（权重绑定）
- **因果自注意力**：Scaled Dot-Product + Causal Mask + Softmax
- **完整的前向与反向传播**，可训练
- **AdamW优化器**（权重衰减解耦、偏差修正）
- **自回归增量推理采样**（带温度）
- **模型二进制保存/加载**
- **莎士比亚字符级数据集内置**（无数据也可直接训练）

## 编译
```bash
make
```

## 快速验证
```bash
# 运行单元测试（过拟合固定batch/保存加载/生成/200步训练）
make run-tests
# 或手动
./build/test_model
```

## 训练（快速演示，约十几秒出可读文本）
```bash
# 小模型快速演示（d=64, 2层, batch=16, 500步）
make demo
# 或自定义参数
./build/train --iters 1000 --d_model 128 --n_heads 4 --n_layers 4 --lr 5e-4 --gen_every 200
```

## 推理生成
```bash
./build/generate --model model.bin --prompt "ROMEO:" --tokens 500 --temp 0.8
```

## 代码结构
- `include/cllm.h` — 基础工具（随机数、矩阵/张量结构、matmul等）
- `include/model.h` — GPT模型公开API
- `include/dataset.h` — 字符级文本数据集接口
- `src/core.c` — 张量/随机数基础（PCG随机、Box-Muller高斯、矩阵乘法）
- `src/dataset.c` — 文本加载、词表构建、batch采样（内置1100字符莎士比亚样本）
- `src/model.c` — **完整的Transformer实现**：
  - 模型创建/释放（He/Xavier初始化）
  - 逐层前向传播（Linear/LayerNorm/GELU/Causal Attention/残差）
  - 逐层反向传播（所有算子梯度、Softmax-CE起点）
  - AdamW参数更新
  - 自回归增量采样（B=1单batch推理）
  - 模型二进制save/load
- `examples/train.c` — 训练主程序（控制台日志、周期生成样例）
- `examples/generate.c` — 交互式推理程序
- `tests/test_model.c` — 单元测试（过拟合验证/保存加载/生成/短训练）

## 默认模型配置
小模型（约**200K参数**，CPU可秒训）：
- d_model=64, heads=4, layers=2, block_size=48, batch_size=16
- 训练1000步后即可生成可读莎士比亚风格文本
