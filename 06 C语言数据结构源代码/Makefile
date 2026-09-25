# C语言数据结构源程序库 Makefile
# 一键编译并运行全部验证程序

CC      = gcc
CFLAGS  = -Wall -Wextra -std=c11 -g -O2 -Iinclude
LDFLAGS =
SRCDIR  = src
TESTDIR = tests
BUILDDIR= build

# 源文件
SRCS = $(wildcard $(SRCDIR)/*.c)
OBJS = $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.o,$(SRCS))

# 测试入口
TEST_MAIN = $(TESTDIR)/test_all.c
TEST_OBJS = $(wildcard $(TESTDIR)/test_*.c)
TARGET   = $(BUILDDIR)/run_all_tests

.PHONY: all run clean dirs

all: dirs $(TARGET)

dirs:
	@mkdir -p $(BUILDDIR)

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS) $(TEST_MAIN) $(TEST_OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
	@echo "✅ 编译成功，目标文件：$(TARGET)"

run: all
	@echo ""
	@./$(TARGET)

clean:
	rm -rf $(BUILDDIR)
