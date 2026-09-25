# C语言数值计算源代码集 - Makefile
# 编译全部8个模块的验证程序
CC = gcc
CFLAGS = -O2 -Wall -std=c11 -Icommon
LDFLAGS = -lm

COMMON_SRC = common/common.c

DIRS = 01_matrix_basics \
       02_linear_solver \
       03_root_finding \
       04_numerical_integration \
       05_interpolation \
       06_ode_solver \
       07_least_squares \
       08_eigenvalue

BINS = 01_matrix_basics/test_matrix \
       02_linear_solver/test_linear \
       03_root_finding/test_root \
       04_numerical_integration/test_integration \
       05_interpolation/test_interpolation \
       06_ode_solver/test_ode \
       07_least_squares/test_lsq \
       08_eigenvalue/test_eigen

.PHONY: all clean run $(DIRS)

all: $(BINS)

# 通用编译规则：每个目录的test_xxx 依赖对应目录.c + common.c
01_matrix_basics/test_matrix: 01_matrix_basics/matrix_ops.c 01_matrix_basics/test_matrix.c $(COMMON_SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

02_linear_solver/test_linear: 02_linear_solver/linear_solver.c 02_linear_solver/test_linear.c $(COMMON_SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

03_root_finding/test_root: 03_root_finding/root_finding.c 03_root_finding/test_root.c $(COMMON_SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

04_numerical_integration/test_integration: 04_numerical_integration/integration.c 04_numerical_integration/test_integration.c $(COMMON_SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

05_interpolation/test_interpolation: 05_interpolation/interpolation.c 05_interpolation/test_interpolation.c $(COMMON_SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

06_ode_solver/test_ode: 06_ode_solver/ode_solver.c 06_ode_solver/test_ode.c $(COMMON_SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

07_least_squares/test_lsq: 07_least_squares/least_squares.c 07_least_squares/test_lsq.c $(COMMON_SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

08_eigenvalue/test_eigen: 08_eigenvalue/eigenvalue.c 08_eigenvalue/test_eigen.c $(COMMON_SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# 运行全部测试
run: all
	@for bin in $(BINS); do \
		echo "\n=========== 运行 $$bin ==========="; \
		(cd `dirname $$bin` && ./`basename $$bin`); \
	done

clean:
	rm -f $(BINS)
