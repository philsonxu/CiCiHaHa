/*
 * Reactor模式极简实现（单Reactor单线程）
 * 核心思想：将IO事件分发给对应事件处理器回调，业务逻辑不阻塞事件循环
 * 组件：
 *   1. Reactor：事件循环核心，封装epoll，负责注册/删除/分发事件
 *   2. EventHandler：事件处理器接口（accept/recv/send）
 *   3. Handle：IO句柄（socket fd）
 * 这是所有高性能网络框架（libevent/libuv/muduo/netty/nginx）的核心设计模式
 * 编译：gcc -o reactor_server reactor_echo_server.c ../common/common.c -I../common
 * 测试：nc 127.0.0.1 8896  或压测
 */
#include "common.h"

#define MAX_EVENTS 4096
#define BUF_SIZE 16384

// 前向声明
typedef struct reactor_s reactor_t;
typedef struct event_handler_s event_handler_t;

// 事件回调类型：handler *this, reactor_t *reactor, int events
typedef void (*event_cb)(event_handler_t*, reactor_t*, int);

struct event_handler_s {
    int fd;
    int events;          // 监听的事件 EPOLLIN/EPOLLOUT/EPOLLET等
    event_cb handle_read;
    event_cb handle_write;
    event_cb handle_close;
    char rbuf[BUF_SIZE];
    size_t rlen;
    char wbuf[BUF_SIZE];
    size_t wlen;
    size_t woff;
    void *data;          // 用户数据
};

struct reactor_s {
    int epfd;
    bool running;
    struct epoll_event events[MAX_EVENTS];
};

reactor_t* reactor_create() {
    reactor_t *r = calloc(1, sizeof(reactor_t));
    r->epfd = epoll_create1(0);
    r->running = true;
    return r;
}

void reactor_register(reactor_t *r, event_handler_t *h) {
    struct epoll_event ev;
    ev.events = h->events;
    ev.data.ptr = h;
    epoll_ctl(r->epfd, EPOLL_CTL_ADD, h->fd, &ev);
}

void reactor_modify(reactor_t *r, event_handler_t *h, int new_events) {
    h->events = new_events;
    struct epoll_event ev;
    ev.events = new_events;
    ev.data.ptr = h;
    epoll_ctl(r->epfd, EPOLL_CTL_MOD, h->fd, &ev);
}

void reactor_remove(reactor_t *r, event_handler_t *h) {
    epoll_ctl(r->epfd, EPOLL_CTL_DEL, h->fd, NULL);
    close(h->fd);
    if (h->handle_close) h->handle_close(h, r, 0);
    free(h);
}

void reactor_run(reactor_t *r) {
    while (r->running) {
        int n = epoll_wait(r->epfd, r->events, MAX_EVENTS, -1);
        if (n < 0) {
            if (errno == EINTR) continue;
            log_error("epoll_wait");
            break;
        }
        for (int i = 0; i < n; i++) {
            event_handler_t *h = (event_handler_t*)r->events[i].data.ptr;
            int ev = r->events[i].events;
            
            if (ev & (EPOLLERR | EPOLLHUP | EPOLLRDHUP)) {
                reactor_remove(r, h);
                continue;
            }
            if ((ev & EPOLLIN) && h->handle_read) {
                h->handle_read(h, r, ev);
            }
            if ((ev & EPOLLOUT) && h->handle_write) {
                h->handle_write(h, r, ev);
            }
        }
    }
}

// ==== 以下为具体业务：Acceptor与Echo处理器 ====

// 前向声明回调
void acceptor_read(event_handler_t*, reactor_t*, int);
void echo_read(event_handler_t*, reactor_t*, int);
void echo_write(event_handler_t*, reactor_t*, int);

event_handler_t* acceptor_create(int port, reactor_t *r) {
    int listenfd = create_tcp_server("0.0.0.0", port, 4096);
    set_nonblocking(listenfd);
    
    event_handler_t *h = calloc(1, sizeof(event_handler_t));
    h->fd = listenfd;
    h->events = EPOLLIN | EPOLLET;
    h->handle_read = acceptor_read;
    h->data = r;
    reactor_register(r, h);
    log_info("Reactor Acceptor启动，端口：%d", port);
    return h;
}

// 处理新连接
void acceptor_read(event_handler_t *acceptor, reactor_t *r, int ev) {
    while (1) {
        struct sockaddr_in cliaddr;
        socklen_t clilen = sizeof(cliaddr);
        int connfd = accept(acceptor->fd, (struct sockaddr*)&cliaddr, &clilen);
        if (connfd < 0) {
            if (errno == EAGAIN) break;
            log_error("accept");
            break;
        }
        set_nonblocking(connfd);
        set_tcp_nodelay(connfd);
        
        event_handler_t *client = calloc(1, sizeof(event_handler_t));
        client->fd = connfd;
        client->events = EPOLLIN | EPOLLRDHUP | EPOLLET;
        client->handle_read = echo_read;
        client->handle_write = echo_write;
        reactor_register(r, client);
        log_debug("新连接 fd=%d", connfd);
    }
}

// 处理客户端读
void echo_read(event_handler_t *h, reactor_t *r, int ev) {
    while (1) {
        size_t avail = sizeof(h->rbuf) - h->rlen;
        if (avail == 0) break;
        ssize_t n = read(h->fd, h->rbuf + h->rlen, avail);
        if (n > 0) {
            h->rlen += n;
        } else if (n == 0) {
            reactor_remove(r, h);
            return;
        } else {
            if (errno == EAGAIN) break;
            reactor_remove(r, h);
            return;
        }
    }
    
    // 收到数据：拷贝到写缓冲区，注册写事件（真正发送交给EPOLLOUT）
    if (h->rlen > 0) {
        // 将读数据追加到写缓冲区尾部（简化版本直接覆盖）
        memcpy(h->wbuf + h->wlen, h->rbuf, h->rlen);
        h->wlen += h->rlen;
        h->rlen = 0;
        if (!(h->events & EPOLLOUT)) {
            reactor_modify(r, h, h->events | EPOLLOUT);
        }
    }
}

// 处理客户端写
void echo_write(event_handler_t *h, reactor_t *r, int ev) {
    while (h->woff < h->wlen) {
        ssize_t n = write(h->fd, h->wbuf + h->woff, h->wlen - h->woff);
        if (n > 0) {
            h->woff += n;
        } else {
            if (errno == EAGAIN) return;
            reactor_remove(r, h);
            return;
        }
    }
    // 全部写完，取消写事件
    h->wlen = 0;
    h->woff = 0;
    reactor_modify(r, h, h->events & ~EPOLLOUT);
}

int main(int argc, char *argv[]) {
    int port = argc > 1 ? atoi(argv[1]) : 8896;
    signal(SIGPIPE, SIG_IGN);
    
    reactor_t *r = reactor_create();
    acceptor_create(port, r);
    log_info("Reactor模式Echo服务器启动");
    reactor_run(r);
    
    close(r->epfd);
    free(r);
    return 0;
}
