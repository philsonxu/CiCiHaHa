CC = gcc
CFLAGS = -O2 -Wall -Wextra -std=c11 -Iinclude -Wno-unused-parameter -Wno-unused-variable -Wno-unused-but-set-variable -Wno-unused-function
LDFLAGS = -lm

SRCDIR = src
TESTDIR = tests
EXDIR = examples
BUILDDIR = build

SRCS = $(wildcard $(SRCDIR)/*.c)
OBJS = $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.o,$(SRCS))
LIB = $(BUILDDIR)/libcllm.a

TESTS = $(BUILDDIR)/test_model
EXAMPLES = $(BUILDDIR)/train $(BUILDDIR)/generate

.PHONY: all clean tests examples run-tests train demo

all: $(LIB) $(TESTS) $(EXAMPLES)

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

$(BUILDDIR)/%.o: $(SRCDIR)/%.c | $(BUILDDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB): $(OBJS)
	ar rcs $@ $^

$(BUILDDIR)/test_%: $(TESTDIR)/test_%.c $(LIB) | $(BUILDDIR)
	$(CC) $(CFLAGS) $< -L$(BUILDDIR) -lcllm $(LDFLAGS) -o $@

$(BUILDDIR)/%: $(EXDIR)/%.c $(LIB) | $(BUILDDIR)
	$(CC) $(CFLAGS) $< -L$(BUILDDIR) -lcllm $(LDFLAGS) -o $@

tests: $(TESTS)
examples: $(EXAMPLES)

run-tests: $(TESTS)
	@echo "Running tests..."
	@for t in $(TESTS); do echo "==> $$t"; $$t || exit 1; done

# 快速演示：小模型500步，几秒训练出可读文本
demo: $(EXAMPLES)
	@mkdir -p data
	$(BUILDDIR)/train --iters 500 --d_model 64 --n_heads 4 --n_layers 2 --batch_size 16 --block_size 48 --lr 8e-4 --gen_every 100 --gen_tokens 150

# 较正式训练：大一点模型2000步
train: $(EXAMPLES)
	@mkdir -p data
	$(BUILDDIR)/train --iters 2000 --d_model 128 --n_heads 4 --n_layers 4 --batch_size 32 --block_size 64 --lr 5e-4 --gen_every 200 --gen_tokens 200

clean:
	rm -rf $(BUILDDIR) model.bin
