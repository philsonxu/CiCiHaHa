/*
 * 多进程并发TCP Echo服务器
 * 原理：主进程accept连接后fork子进程专门处理该客户端
 * 特点：进程间隔离，一个客户端崩溃不影响其他；但进程资源开销大，并发数有限
 * 僵尸进程处理：通过signal(SIGCHLD, SIG_IGN)让内核自动回收子进程
 * 编译：gcc -o tcp_echo_fork tcp_echo_server_fork.c ../common/common.c -I../common
 * 测试：同时开多个 nc 127.0.0.1 8889 可同时回显互不阻塞
 */
#include "common.h"
#include <sys/wait.h>

void sigchld_handler(int sig) {
    // 非阻塞回收所有已退出子进程
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

void echo_service(int connfd) {
    char buf[4096];
    char ip[INET_ADDRSTRLEN];
    int port;
    get_peer_addr(connfd, ip, &port);
    
    while (1) {
        ssize_t n = read(connfd, buf, sizeof(buf)-1);
        if (n > 0) {
            buf[n] = '\0';
            log_info("[子进程%d] 来自%s:%d: %s", getpid(), ip, port, buf);
            writen(connfd, buf, n);
        } else if (n == 0) {
            log_info("[子进程%d] 客户端断开%s:%d", getpid(), ip, port);
            break;
        } else {
            if (errno == EINTR) continue;
            break;
        }
    }
    close(connfd);
}

int main(int argc, char *argv[]) {
    int port = argc > 1 ? atoi(argv[1]) : 8889;
    
    signal(SIGPIPE, SIG_IGN);
    signal(SIGCHLD, sigchld_handler); // 处理僵尸进程
    
    int listenfd = create_tcp_server("0.0.0.0", port, 128);
    log_info("多进程Echo服务器启动，端口：%d", port);
    
    while (1) {
        struct sockaddr_in cliaddr;
        socklen_t clilen = sizeof(cliaddr);
        int connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &clilen);
        if (connfd < 0) {
            if (errno == EINTR) continue;
            log_error("accept");
            continue;
        }
        
        pid_t pid = fork();
        if (pid == 0) {
            // 子进程：关闭监听套接字，只处理当前连接
            close(listenfd);
            echo_service(connfd);
            exit(EXIT_SUCCESS); // 子进程处理完退出
        } else if (pid > 0) {
            // 父进程：关闭已连接套接字，继续accept
            close(connfd);
        } else {
            log_error("fork");
            close(connfd);
        }
    }
    
    close(listenfd);
    return 0;
}
