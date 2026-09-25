/**
 * ============================================================================
 * C语言信号处理
 * 说明：信号是进程间异步通知机制，是Linux异步编程的基础
 * 编译：gcc -Wall -std=c99 -o signal_demo signal_demo.c
 * 测试：运行程序后按 Ctrl+C 或 Ctrl+\，或另一个终端 kill <pid>
 * ============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

volatile sig_atomic_t stop_flag = 0;    // 原子变量，信号安全
volatile sig_atomic_t alarm_fired = 0;

/* SIGINT 处理函数（Ctrl+C） */
void sigint_handler(int sig) {
    printf("\n[信号] 收到 SIGINT(%d)，再次按 Ctrl+C 退出\n", sig);
    // 第一次收到时重置为默认处理，第二次则真的退出
    signal(SIGINT, SIG_DFL);
}

/* SIGTERM 处理函数（kill 默认信号） */
void sigterm_handler(int sig) {
    printf("\n[信号] 收到 SIGTERM(%d)，准备优雅退出...\n", sig);
    stop_flag = 1;
}

/* SIGALRM 处理函数：定时器 */
void sigalrm_handler(int sig) {
    (void)sig;
    alarm_fired = 1;
    alarm(1);   // 重新设置1秒定时器
}

/* 使用 sigaction 注册信号（推荐方式，比 signal() 可靠） */
void register_signal(int sig, void (*handler)(int)) {
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(sig, &sa, NULL);
}

int main() {
    printf("=======================================\n");
    printf("   C语言信号处理演示                   \n");
    printf("=======================================\n\n");

    printf("本进程 PID = %d\n", getpid());
    printf("操作说明：\n");
    printf("  Ctrl+C     : 触发 SIGINT\n");
    printf("  Ctrl+\\     : 触发 SIGQUIT（默认核心转储）\n");
    printf("  kill %d    : 触发 SIGTERM 优雅退出\n", getpid());
    printf("程序将每秒打印一次心跳...\n\n");

    // 注册信号处理函数
    register_signal(SIGINT, sigint_handler);
    register_signal(SIGTERM, sigterm_handler);
    register_signal(SIGALRM, sigalrm_handler);

    // 阻塞 SIGQUIT 信号（演示信号掩码）
    sigset_t block_set, old_set;
    sigemptyset(&block_set);
    sigaddset(&block_set, SIGQUIT);
    sigprocmask(SIG_BLOCK, &block_set, &old_set);
    printf("已暂时屏蔽 SIGQUIT，5秒后解除屏蔽...\n");

    // 启动1秒定时器
    alarm(1);

    int heartbeat = 0;
    time_t start = time(NULL);

    while (!stop_flag) {
        pause();  // 挂起进程直到收到信号（比忙循环节省CPU）

        if (alarm_fired) {
            alarm_fired = 0;
            heartbeat++;
            printf("[心跳] %d 秒\n", heartbeat);
            fflush(stdout);
        }

        // 5秒后解除 SIGQUIT 屏蔽
        if (heartbeat == 5) {
            sigprocmask(SIG_SETMASK, &old_set, NULL);
            printf("\n已解除 SIGQUIT 屏蔽，现在可以按 Ctrl+\\\n\n");
        }
    }

    printf("\n资源清理中...\n");
    sleep(1);
    printf("程序正常退出。运行时长：%ld 秒\n", time(NULL) - start);
    printf("\n常见信号说明：\n");
    printf("  SIGHUP(1)  : 终端断开\n");
    printf("  SIGINT(2)  : Ctrl+C 中断\n");
    printf("  SIGQUIT(3) : Ctrl+\\ 退出并转储\n");
    printf("  SIGKILL(9) : 强制杀死（无法捕获）\n");
    printf("  SIGSEGV(11): 段错误\n");
    printf("  SIGALRM(14): 闹钟定时器\n");
    printf("  SIGTERM(15): kill 默认终止\n");
    printf("  SIGCHLD(17): 子进程停止/退出\n");

    return 0;
}
