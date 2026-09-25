/*
 * 多线程并发TCP Echo服务器
 * 原理：主accept后创建线程处理，线程共享进程地址空间
 * 注意：connfd必须动态分配或按值传递，避免多线程竞争同一个fd变量
 * 特点：线程开销比进程小，上下文切换快；需注意线程安全与资源回收
 * 编译：gcc -o tcp_echo_pthread tcp_echo_server_pthread.c ../common/common.c -I../common -lpthread
 * 测试：同时开多个 nc 127.0.0.1 8890
 */
#include "common.h"

typedef struct {
    int connfd;
    struct sockaddr_in cliaddr;
} thread_arg_t;

void* echo_thread(void *arg) {
    thread_arg_t *targ = (thread_arg_t*)arg;
    int connfd = targ->connfd;
    char ip[INET_ADDRSTRLEN];
    int port;
    get_peer_addr(connfd, ip, &port);
    pthread_detach(pthread_self()); // 自动回收线程资源，无需pthread_join
    free(targ);
    
    char buf[4096];
    while (1) {
        ssize_t n = read(connfd, buf, sizeof(buf)-1);
        if (n > 0) {
            buf[n] = '\0';
            log_info("[线程%lu] %s:%d: %s", pthread_self(), ip, port, buf);
            writen(connfd, buf, n);
        } else if (n == 0) {
            log_info("[线程%lu] 客户端断开%s:%d", pthread_self(), ip, port);
            break;
        } else {
            if (errno == EINTR) continue;
            break;
        }
    }
    close(connfd);
    return NULL;
}

int main(int argc, char *argv[]) {
    int port = argc > 1 ? atoi(argv[1]) : 8890;
    
    signal(SIGPIPE, SIG_IGN);
    
    int listenfd = create_tcp_server("0.0.0.0", port, 128);
    log_info("多线程Echo服务器启动，端口：%d", port);
    
    while (1) {
        thread_arg_t *targ = malloc(sizeof(thread_arg_t));
        socklen_t clilen = sizeof(targ->cliaddr);
        targ->connfd = accept(listenfd, (struct sockaddr*)&targ->cliaddr, &clilen);
        if (targ->connfd < 0) {
            free(targ);
            if (errno == EINTR) continue;
            log_error("accept");
            continue;
        }
        
        pthread_t tid;
        if (pthread_create(&tid, NULL, echo_thread, targ) != 0) {
            log_error("pthread_create");
            close(targ->connfd);
            free(targ);
        }
    }
    
    close(listenfd);
    return 0;
}
