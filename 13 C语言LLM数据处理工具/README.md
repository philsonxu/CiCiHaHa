# LLM 数据处理 C 语言工具集

把原先用 Python 写的两条数据处理管线（`clean_merge.py`、`build_tokenizer.py`）
重写为纯 C 实现，**无需 Python 环境、无任何第三方依赖**，只需要 gcc + 标准 C 库。

## 编译

```bash
make
```

会在 `bin/` 目录下生成两个可执行文件：
- `bin/clean_merge`     数据清洗 + 训练/验证划分
- `bin/build_tokenizer` 字符级词表构建 + tokenize 二进制化

## 使用流程

### 第 1 步：准备原始文本

把所有 `.txt` 原始语料放到 `data/raw/zh/` 和 `data/raw/en/` 下（子目录可以任意嵌套），
例如：

```
data/raw/zh/红楼梦.txt
data/raw/zh/史记.txt
data/raw/en/shakespeare.txt
data/raw/en/frankenstein.txt
```

### 第 2 步：清洗并划分 train/eval

```bash
./bin/clean_merge                        # 使用默认参数
./bin/clean_merge --raw_dir data/raw --val_ratio 0.05 --seed 42
```

输出：
- `data/pretrain/train.txt`   训练文本（文档间用 `<|endoftext|>\n` 分隔）
- `data/eval/eval.txt`        验证文本（默认占 5%）

清洗规则：
1. 自动去除空行、控制字符（< 0x20、0x7f、0x0d）
2. 把连续空白压缩成单个空格
3. 自动跳过 Project Gutenberg 书籍开头/结尾的版权声明
   （识别 `*** START OF ... ***` 与 `*** END OF ... ***` 标记）
4. **文档级 Fisher-Yates 随机打乱**，避免某本书全部落在 train 或 eval 中
5. 切分点对齐到文档边界，不会把一篇文档切断

### 第 3 步：构建词表 + Tokenize 成二进制

```bash
./bin/build_tokenizer                    # 默认参数
./bin/build_tokenizer --text_dir data --out_dir data/tokenized \
                      --max_vocab 8000   \
                      --tokenizer_path tokenizer/char_tokenizer.json
```

输出：
- `tokenizer/char_tokenizer.json`  词表文件（含 `vocab_size`、`special_tokens`、
  `itos` 列表、`stoi` 反向字典），UTF-8 直接保存，中文/英文/标点都是原字符。
- `data/tokenized/train.bin`       uint16 小端序 token id 序列
- `data/tokenized/eval.bin`

特殊 token id 约定：

| id | token          | 用途                |
|----|----------------|---------------------|
| 0  | `<pad>`        | 填充                |
| 1  | `<unk>`        | 未登录字符          |
| 2  | `<bos>`        | 序列起始            |
| 3  | `<eos>`        | 序列结束            |
| 4  | `<|endoftext|>`| 文档分隔符 (GPT 标准)|

词表按字符频次降序排列，从 id=5 开始是真实字符，最多 `max_vocab - 5` 个。

## 模型侧读取示例（C 语言）

```c
// 直接 mmap 读取 token 二进制
FILE *fp = fopen("data/tokenized/train.bin", "rb");
fseek(fp, 0, SEEK_END); long sz = ftell(fp); fseek(fp, 0, SEEK_SET);
uint16_t *tokens = malloc(sz);
fread(tokens, 1, sz, fp); fclose(fp);
long n_tokens = sz / 2;
// n_tokens 个 token id，随机起点切块即可喂给模型
```

Python 侧读取：

```python
import numpy as np
train = np.fromfile("data/tokenized/train.bin", dtype=np.uint16)
val   = np.fromfile("data/tokenized/eval.bin",  dtype=np.uint16)
```

## 命令行参数一览

`clean_merge`:
| 参数          | 默认       | 说明                |
|---------------|------------|---------------------|
| `--raw_dir`   | data/raw   | 原始 .txt 根目录    |
| `--out_dir`   | data       | train/eval 输出目录 |
| `--val_ratio` | 0.05       | 验证集比例          |
| `--seed`      | 时间戳     | 随机种子（可复现）  |

`build_tokenizer`:
| 参数               | 默认                           | 说明                  |
|--------------------|--------------------------------|-----------------------|
| `--text_dir`       | data                           | train.txt/eval.txt 所在目录 |
| `--out_dir`        | data/tokenized                 | bin 输出目录          |
| `--tokenizer_path` | tokenizer/char_tokenizer.json  | 词表 JSON 路径        |
| `--max_vocab`      | 8000                           | 最大词表大小          |

## 实现说明

- **UTF-8**：自实现解码（根据首字节高位 1 的个数识别 1/2/3/4 字节字符），
  完全支持中日韩文、emoji，不依赖 iconv/ICU。
- **哈希表**：stoi/cnt 采用线性探查哈希表 + FNV-1a 哈希，统计/查询 O(1)。
- **JSON 写出**：自实现 JSON 字符串转义（引号、反斜杠、控制字符 \uXXXX），
  UTF-8 多字节字符按原文写，Python/JavaScript/C++ 均可直接 `json.load` 读取。
- **小端序 uint16**：x86/ARM 默认都是小端，直接 `fwrite(uint16_t)` 写出，
  C/Python/numpy 都能直接读。
