/*
 * Epoll ET(边缘触发)模式高性能Echo服务器
 * ET模式：只有fd状态发生变化时才通知一次（从无数据到有数据），必须一次性读完/写完所有数据
 * 关键要求：
 *   1. 所有fd必须是非阻塞的
 *   2. read必须循环读到EAGAIN，write必须循环写到EAGAIN，否则会丢失事件导致饥饿
 *   3. 写操作一般要等EPOLLOUT事件（写缓冲区可写时才通知）
 * 优势：事件通知次数大大减少，性能高于LT；大并发高吞吐场景首选
 * 这是Nginx/Redis/Netty等高性能框架的核心IO模型
 * 编译：gcc -o epoll_et_server epoll_server_et.c ../common/common.c -I../common
 * 测试：nc 127.0.0.1 8895  可用压测客户端 ./tcp_benchmark_client 127.0.0.1 8895 100 10000
 */
#include "common.h"

#define MAX_EVENTS 4096
#define BUF_SIZE 8192

typedef struct conn_s {
    int fd;
    char rbuf[BUF_SIZE];
    size_t rlen;       // 已读字节数
    char wbuf[BUF_SIZE];
    size_t wlen;       // 待写字节数
    size_t woff;       // 已写偏移
    struct conn_s *next;
} conn_t;

conn_t *conn_alloc(int fd) {
    conn_t *c = calloc(1, sizeof(conn_t));
    c->fd = fd;
    return c;
}

void conn_free(conn_t *c) {
    close(c->fd);
    free(c);
}

int main(int argc, char *argv[]) {
    int port = argc > 1 ? atoi(argv[1]) : 8895;
    signal(SIGPIPE, SIG_IGN);
    
    int listenfd = create_tcp_server("0.0.0.0", port, 4096);
    set_nonblocking(listenfd);
    set_tcp_nodelay(listenfd);
    
    int epfd = epoll_create1(0);
    if (epfd < 0) ERR_EXIT("epoll_create1");
    
    struct epoll_event ev, events[MAX_EVENTS];
    // ET模式必须加EPOLLET标志
    ev.events = EPOLLIN | EPOLLET;
    ev.data.ptr = conn_alloc(listenfd);
    epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &ev);
    
    log_info("Epoll ET高性能服务器启动，端口：%d", port);
    
    while (1) {
        int nready = epoll_wait(epfd, events, MAX_EVENTS, -1);
        if (nready < 0) {
            if (errno == EINTR) continue;
            log_error("epoll_wait");
            break;
        }
        
        for (int i = 0; i < nready; i++) {
            conn_t *c = (conn_t*)events[i].data.ptr;
            int fd = c->fd;
            
            if (fd == listenfd) {
                // ET模式必须循环accept到EAGAIN，否则新连接会丢失
                while (1) {
                    struct sockaddr_in cliaddr;
                    socklen_t clilen = sizeof(cliaddr);
                    int connfd = accept(fd, (struct sockaddr*)&cliaddr, &clilen);
                    if (connfd < 0) {
                        if (errno == EAGAIN || errno == EWOULDBLOCK) break;
                        log_error("accept");
                        break;
                    }
                    set_nonblocking(connfd);
                    set_tcp_nodelay(connfd);
                    conn_t *nc = conn_alloc(connfd);
                    struct epoll_event nev;
                    nev.events = EPOLLIN | EPOLLRDHUP | EPOLLET;
                    nev.data.ptr = nc;
                    epoll_ctl(epfd, EPOLL_CTL_ADD, connfd, &nev);
                    log_debug("新连接 fd=%d", connfd);
                }
                continue;
            }
            
            // 客户端出错/断开
            if (events[i].events & (EPOLLERR | EPOLLHUP | EPOLLRDHUP)) {
                epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
                conn_free(c);
                continue;
            }
            
            // ET模式读：必须循环读直到EAGAIN，一次性读完缓冲区所有数据
            if (events[i].events & EPOLLIN) {
                while (1) {
                    size_t avail = sizeof(c->rbuf) - c->rlen;
                    if (avail == 0) {
                        log_warn("fd=%d 读缓冲区满", fd);
                        break;
                    }
                    ssize_t n = read(fd, c->rbuf + c->rlen, avail);
                    if (n > 0) {
                        c->rlen += n;
                        log_debug("fd=%d 读入%zd字节, 累计%zu", fd, n, c->rlen);
                    } else if (n == 0) {
                        // 对端关闭
                        epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
                        conn_free(c);
                        goto next_event;
                    } else {
                        if (errno == EAGAIN || errno == EWOULDBLOCK) break; // 读完了
                        if (errno == EINTR) continue;
                        epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
                        conn_free(c);
                        goto next_event;
                    }
                }
                
                // 简单Echo：把rbuf数据拷贝到wbuf，准备发送，注册EPOLLOUT
                if (c->rlen > 0) {
                    memcpy(c->wbuf, c->rbuf, c->rlen);
                    c->wlen = c->rlen;
                    c->woff = 0;
                    c->rlen = 0;
                    // 修改事件关注写
                    struct epoll_event wev;
                    wev.events = EPOLLIN | EPOLLOUT | EPOLLRDHUP | EPOLLET;
                    wev.data.ptr = c;
                    epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &wev);
                }
            }
            
            // ET模式写：循环写到EAGAIN或全部写完
            if (events[i].events & EPOLLOUT) {
                while (c->woff < c->wlen) {
                    ssize_t n = write(fd, c->wbuf + c->woff, c->wlen - c->woff);
                    if (n > 0) {
                        c->woff += n;
                    } else {
                        if (errno == EAGAIN || errno == EWOULDBLOCK) break; // 写缓冲区满，等下次通知
                        if (errno == EINTR) continue;
                        epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
                        conn_free(c);
                        goto next_event;
                    }
                }
                if (c->woff >= c->wlen) {
                    // 全部写完，取消关注EPOLLOUT（否则持续写通知会busy loop）
                    c->wlen = 0;
                    c->woff = 0;
                    struct epoll_event rev;
                    rev.events = EPOLLIN | EPOLLRDHUP | EPOLLET;
                    rev.data.ptr = c;
                    epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &rev);
                }
            }
next_event:;
        }
    }
    
    close(epfd);
    close(listenfd);
    return 0;
}
