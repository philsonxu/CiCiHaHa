# C语言高级编程源代码集

本套代码涵盖C语言高级编程的核心主题，每个示例均可独立编译运行。

## 目录结构

| 目录 | 主题 | 核心内容 |
|------|------|----------|
| 01_pointer_advanced | 指针高级用法 | 二级指针、函数指针、void指针、指针运算、数组指针 |
| 02_memory_management | 内存管理 | 动态内存分配、内存池实现、常见内存错误、malloc实现原理 |
| 03_data_structures | 高级数据结构 | 链表、栈、队列、哈希表、二叉搜索树 |
| 04_multithreading | 多线程编程 | 线程创建、互斥锁、条件变量、读写锁、线程池 |
| 05_process_ipc | 进程与通信 | fork进程、管道、共享内存、消息队列、信号量 |
| 06_network_programming | 网络编程 | TCP/UDP套接字、IO多路复用(select/poll/epoll)、Reactor模式 |
| 07_file_io | 文件IO高级操作 | 标准IO、系统调用IO、mmap内存映射、文件锁 |
| 08_signal_handling | 信号处理 | 信号注册、信号集、sigaction、定时器 |
| 09_preprocessor_macros | 预处理与宏 | 宏定义技巧、条件编译、泛型宏、X-Macro |
| 10_callback_design_pattern | 回调与设计模式 | 回调函数、策略模式、观察者模式、状态机 |
| 11_performance_optimization | 性能优化 | 位运算、循环优化、内存对齐、内联函数 |

## 编译方法

每个目录下都有独立的Makefile，进入对应目录执行：
```bash
make          # 编译所有示例
make clean    # 清理编译产物
```

或在根目录执行：
```bash
make all      # 编译所有章节的示例
```

## 编译环境

- GCC 4.8+ (支持C99/C11)
- Linux环境（多线程、网络、IPC部分需要POSIX支持）
- 编译命令：`gcc -Wall -std=c99 -o output source.c -lpthread`

## 学习建议

1. 按目录顺序学习，从指针基础开始
2. 阅读代码中的详细注释
3. 手动编译运行每个示例
4. 修改参数并观察输出变化
5. 尝试扩展功能进行练习
