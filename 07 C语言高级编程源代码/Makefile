# C语言高级编程源代码集 - 根Makefile
# 使用：make [all|clean]

CC      = gcc
CFLAGS  = -Wall -std=gnu99 -O2 -D_GNU_SOURCE
LDFLAGS =
LDLIBS  = -lpthread -lrt

DIRS = 01_pointer_advanced \
       02_memory_management \
       03_data_structures \
       04_multithreading \
       05_process_ipc \
       06_network_programming \
       07_file_io \
       08_signal_handling \
       09_preprocessor_macros \
       10_callback_design_pattern \
       11_performance_optimization

# 各目录的目标程序
EXES = 01_pointer_advanced/pointer_advanced \
       02_memory_management/memory_pool \
       03_data_structures/hash_table \
       04_multithreading/thread_pool \
       05_process_ipc/shared_memory \
       06_network_programming/tcp_epoll_server \
       07_file_io/mmap_demo \
       08_signal_handling/signal_demo \
       09_preprocessor_macros/macro_advanced \
       10_callback_design_pattern/observer_pattern \
       11_performance_optimization/perf_optimization

.PHONY: all clean help $(DIRS)

all: $(EXES)
	@echo ""
	@echo "========================================="
	@echo " 全部示例编译成功！"
	@echo "========================================="
	@echo ""
	@echo "运行示例（例）："
	@echo "  cd 01_pointer_advanced && ./pointer_advanced"
	@echo "  cd 06_network_programming && ./tcp_epoll_server"
	@echo ""

# 通用编译规则
01_pointer_advanced/pointer_advanced: 01_pointer_advanced/pointer_advanced.c
	$(CC) $(CFLAGS) -o $@ $^

02_memory_management/memory_pool: 02_memory_management/memory_pool.c
	$(CC) $(CFLAGS) -o $@ $^

03_data_structures/hash_table: 03_data_structures/hash_table.c
	$(CC) $(CFLAGS) -o $@ $^

04_multithreading/thread_pool: 04_multithreading/thread_pool.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

05_process_ipc/shared_memory: 05_process_ipc/shared_memory.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

06_network_programming/tcp_epoll_server: 06_network_programming/tcp_epoll_server.c
	$(CC) $(CFLAGS) -o $@ $^

07_file_io/mmap_demo: 07_file_io/mmap_demo.c
	$(CC) $(CFLAGS) -o $@ $^

08_signal_handling/signal_demo: 08_signal_handling/signal_demo.c
	$(CC) $(CFLAGS) -o $@ $^

09_preprocessor_macros/macro_advanced: 09_preprocessor_macros/macro_advanced.c
	$(CC) $(CFLAGS) -o $@ $^

10_callback_design_pattern/observer_pattern: 10_callback_design_pattern/observer_pattern.c
	$(CC) $(CFLAGS) -o $@ $^

11_performance_optimization/perf_optimization: 11_performance_optimization/perf_optimization.c
	$(CC) $(CFLAGS) -O0 -o $@ $^   # 性能测试用-O0避免被编译器优化

clean:
	rm -f $(EXES)
	@echo "已清理所有编译产物"

help:
	@echo "C语言高级编程源代码集"
	@echo ""
	@echo "可用目标："
	@echo "  make       - 编译所有示例"
	@echo "  make clean - 清理编译产物"
	@echo "  make help  - 显示本帮助"
