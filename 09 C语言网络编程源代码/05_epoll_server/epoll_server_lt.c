/*
 * Epoll LT(水平触发)模式Echo服务器
 * LT模式：只要fd缓冲区有数据可读，epoll_wait就会持续返回该事件（类似select/poll）
 * 特点：编程简单，不容易漏事件；但事件可能重复通知，稍低效
 * epoll vs select/poll优势：
 * 1. 没有FD上限，支持数十万并发
 * 2. 基于事件回调，只返回活跃fd，无需遍历全部
 * 3. 内核与用户空间共享内存，减少拷贝开销
 * 编译：gcc -o epoll_lt_server epoll_server_lt.c ../common/common.c -I../common
 * 测试：开多个 nc 127.0.0.1 8894
 */
#include "common.h"

#define MAX_EVENTS 1024

struct client_conn {
    int fd;
    char buf[4096];
};

int main(int argc, char *argv[]) {
    int port = argc > 1 ? atoi(argv[1]) : 8894;
    signal(SIGPIPE, SIG_IGN);
    
    int listenfd = create_tcp_server("0.0.0.0", port, 1024);
    set_nonblocking(listenfd);
    
    int epfd = epoll_create1(0);
    if (epfd < 0) ERR_EXIT("epoll_create1");
    
    struct epoll_event ev, events[MAX_EVENTS];
    ev.events = EPOLLIN;
    ev.data.fd = listenfd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &ev);
    
    log_info("Epoll LT模式服务器启动，端口：%d", port);
    
    while (1) {
        int nready = epoll_wait(epfd, events, MAX_EVENTS, -1);
        if (nready < 0) {
            if (errno == EINTR) continue;
            log_error("epoll_wait");
            break;
        }
        
        for (int i = 0; i < nready; i++) {
            int fd = events[i].data.fd;
            
            if (fd == listenfd) {
                // 处理新连接
                while (1) { // LT模式可accept一个，ET必须循环accept到EAGAIN
                    struct sockaddr_in cliaddr;
                    socklen_t clilen = sizeof(cliaddr);
                    int connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &clilen);
                    if (connfd < 0) break;
                    
                    set_nonblocking(connfd);
                    struct client_conn *c = malloc(sizeof(struct client_conn));
                    c->fd = connfd;
                    ev.events = EPOLLIN | EPOLLRDHUP;
                    ev.data.ptr = c;
                    epoll_ctl(epfd, EPOLL_CTL_ADD, connfd, &ev);
                    
                    char ip[INET_ADDRSTRLEN];
                    int p;
                    get_peer_addr(connfd, ip, &p);
                    log_info("新连接 fd=%d %s:%d", connfd, ip, p);
                }
            } else {
                // 客户端数据可读
                struct client_conn *c = (struct client_conn*)events[i].data.ptr;
                int cfd = c->fd;
                
                if (events[i].events & (EPOLLERR | EPOLLHUP | EPOLLRDHUP)) {
                    log_info("fd=%d 断开", cfd);
                    close(cfd);
                    epoll_ctl(epfd, EPOLL_CTL_DEL, cfd, NULL);
                    free(c);
                    continue;
                }
                
                if (events[i].events & EPOLLIN) {
                    ssize_t n = read(cfd, c->buf, sizeof(c->buf));
                    if (n > 0) {
                        c->buf[n] = '\0';
                        log_info("fd=%d: %s", cfd, c->buf);
                        writen(cfd, c->buf, n);
                    } else if (n <= 0) {
                        log_info("fd=%d 断开", cfd);
                        close(cfd);
                        epoll_ctl(epfd, EPOLL_CTL_DEL, cfd, NULL);
                        free(c);
                    }
                }
            }
        }
    }
    
    close(epfd);
    close(listenfd);
    return 0;
}
