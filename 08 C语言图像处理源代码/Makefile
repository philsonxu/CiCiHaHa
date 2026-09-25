# C语言图像处理库 Makefile
CC      := gcc
CFLAGS  := -std=c11 -Wall -Wextra -O2 -Iinclude -g -D_GNU_SOURCE -D_POSIX_C_SOURCE=200809L
LDFLAGS := -lm

# 核心库源文件
LIB_SRC := src/core.c src/bmp_io.c src/ppm_pgm_io.c src/adjust.c src/geometry.c \
           src/histogram.c src/filter.c src/morphology.c src/draw.c src/noise_stat.c
LIB_OBJ := $(LIB_SRC:.c=.o)
LIB_A   := libimageproc.a

# 示例程序
EXAMPLES := examples/demo_basic examples/demo_filters examples/demo_edges \
            examples/demo_morphology examples/demo_noise_denoise \
            examples/demo_geometry examples/demo_paint examples/demo_histeq

# 测试程序
TESTS := tests/test_io tests/test_filters tests/test_pipeline

.PHONY: all clean examples tests run run-tests

all: $(LIB_A) examples tests

$(LIB_A): $(LIB_OBJ)
	ar rcs $@ $^

%.o: %.c include/imageproc.h
	$(CC) $(CFLAGS) -c $< -o $@

examples/%: examples/%.c $(LIB_A)
	$(CC) $(CFLAGS) $< -L. $(LIB_A) $(LDFLAGS) -o $@

tests/%: tests/%.c $(LIB_A)
	$(CC) $(CFLAGS) $< -L. $(LIB_A) $(LDFLAGS) -o $@

examples: $(EXAMPLES)
tests: $(TESTS)

run: examples
	@echo "========== 运行全部示例 =========="
	@mkdir -p output
	@for ex in $(EXAMPLES); do echo ">>> $$ex"; ./$$ex; done
	@ls -la output/

run-tests: tests
	@echo "========== 运行验证程序 =========="
	@for t in $(TESTS); do echo ">>> $$t"; ./$$t; done

clean:
	rm -f $(LIB_OBJ) $(LIB_A)
	rm -f $(EXAMPLES) $(TESTS)
	rm -rf output
