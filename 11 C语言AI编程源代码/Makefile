# ============================================================
# C语言AI编程源代码库 - Makefile
# 纯C实现, 无第三方依赖, 仅依赖标准数学库(-lm)与pthread(预留)
# ============================================================

CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c11 -Iinclude
LDFLAGS = -lm -lpthread

SRC_DIR = src
INC_DIR = include
TEST_DIR = tests
EXAMPLE_DIR = examples
BUILD_DIR = build

# 源文件 (所有src下.c)
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))
LIB = $(BUILD_DIR)/libcai.a

# 测试程序
TESTS = test_classic_ml test_nn test_cnn
# 示例程序
EXAMPLES = example_all_models example_mnist_train

.PHONY: all clean tests examples run run-tests run-examples

all: $(LIB) tests examples

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# 打包静态库
$(LIB): $(OBJS)
	ar rcs $@ $^

# 通用测试/示例编译规则
$(BUILD_DIR)/%: $(TEST_DIR)/%.c $(LIB) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -L$(BUILD_DIR) -lcai $(LDFLAGS) -o $@

$(BUILD_DIR)/example_%: $(EXAMPLE_DIR)/example_%.c $(LIB) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -L$(BUILD_DIR) -lcai $(LDFLAGS) -o $@

tests: $(patsubst %,$(BUILD_DIR)/%,$(TESTS))

examples: $(patsubst %,$(BUILD_DIR)/%,$(EXAMPLES))

run-tests: tests
	@echo ""
	@echo "========== 运行经典ML验证 =========="
	@$(BUILD_DIR)/test_classic_ml
	@echo ""
	@echo "========== 运行神经网络验证 =========="
	@$(BUILD_DIR)/test_nn
	@echo ""
	@echo "========== 运行CNN验证 =========="
	@$(BUILD_DIR)/test_cnn

run-examples: examples
	@echo ""
	@echo "========== 全模型综合演示 =========="
	@$(BUILD_DIR)/example_all_models

run: run-tests run-examples

clean:
	rm -rf $(BUILD_DIR)
	rm -f *.bin

# 快捷编译调试版 (开警告+gdb)
debug: CFLAGS += -g -O0 -DDEBUG
debug: clean all
