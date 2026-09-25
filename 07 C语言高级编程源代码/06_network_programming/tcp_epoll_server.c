/**
 * ============================================================================
 * C语言高性能 TCP 服务器（epoll 反应堆模式）
 * 说明：基于 Linux epoll 实现的 IO 多路复用并发服务器，可处理上万连接
 * 编译：gcc -Wall -std=c99 -o tcp_epoll_server tcp_epoll_server.c
 * 使用：在一个终端运行 ./tcp_epoll_server，另一个终端 telnet 127.0.0.1 8888
 * ============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>

#define PORT          8888
#define MAX_EVENTS    1024
#define BUF_SIZE      1024
#define MAX_CLIENTS   10000

/* 设置文件描述符为非阻塞 */
int set_nonblocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) return -1;
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

/* 向 epoll 注册 fd */
void epoll_add(int epfd, int fd, uint32_t events) {
    struct epoll_event ev;
    ev.events = events;
    ev.data.fd = fd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);
}

int main() {
    printf("=======================================\n");
    printf("   epoll 高并发 TCP 服务器（端口 %d）\n", PORT);
    printf("=======================================\n\n");

    // 1. 创建监听 socket
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd < 0) { perror("socket"); exit(1); }

    int opt = 1;
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(listen_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind"); exit(1);
    }
    if (listen(listen_fd, SOMAXCONN) < 0) {
        perror("listen"); exit(1);
    }

    set_nonblocking(listen_fd);
    printf("服务器启动，监听 0.0.0.0:%d ...\n\n", PORT);

    // 2. 创建 epoll 实例
    int epfd = epoll_create1(0);
    if (epfd < 0) { perror("epoll_create1"); exit(1); }

    epoll_add(epfd, listen_fd, EPOLLIN | EPOLLET);  // 边缘触发

    struct epoll_event events[MAX_EVENTS];
    char buf[BUF_SIZE];
    int client_count = 0;

    while (1) {
        // 3. 等待事件（-1 表示永久阻塞直到有事件）
        int nready = epoll_wait(epfd, events, MAX_EVENTS, -1);
        if (nready < 0) { perror("epoll_wait"); break; }

        for (int i = 0; i < nready; i++) {
            int fd = events[i].data.fd;

            // 3.1 新连接到来
            if (fd == listen_fd) {
                struct sockaddr_in client_addr;
                socklen_t addr_len = sizeof(client_addr);
                int conn_fd;
                while ((conn_fd = accept(listen_fd, (struct sockaddr *)&client_addr, &addr_len)) > 0) {
                    if (client_count >= MAX_CLIENTS) {
                        close(conn_fd);
                        printf("客户端数量已达上限，拒绝连接\n");
                        continue;
                    }
                    set_nonblocking(conn_fd);
                    epoll_add(epfd, conn_fd, EPOLLIN | EPOLLET | EPOLLRDHUP);
                    client_count++;
                    printf("新客户端连接: %s:%d (fd=%d, 总连接=%d)\n",
                           inet_ntoa(client_addr.sin_addr),
                           ntohs(client_addr.sin_port), conn_fd, client_count);

                    const char *welcome = "欢迎连接 epoll 服务器！\r\n";
                    send(conn_fd, welcome, strlen(welcome), 0);
                }
            }
            // 3.2 客户端可读事件（边缘触发需要循环读直到 EAGAIN）
            else if (events[i].events & EPOLLIN) {
                while (1) {
                    memset(buf, 0, BUF_SIZE);
                    ssize_t n = recv(fd, buf, BUF_SIZE - 1, 0);
                    if (n > 0) {
                        printf("[fd=%d] 收到: %s", fd, buf);
                        // Echo：原样返回
                        send(fd, "Echo: ", 6, 0);
                        send(fd, buf, n, 0);
                    } else if (n == 0) {
                        // 对方关闭连接
                        printf("[fd=%d] 客户端断开连接\n", fd);
                        close(fd);
                        client_count--;
                        break;
                    } else {
                        if (errno == EAGAIN || errno == EWOULDBLOCK) {
                            break;  // 本次数据已读完
                        }
                        printf("[fd=%d] recv 错误\n", fd);
                        close(fd);
                        client_count--;
                        break;
                    }
                }
            }
            // 3.3 对端挂起或错误
            else if (events[i].events & (EPOLLRDHUP | EPOLLERR | EPOLLHUP)) {
                printf("[fd=%d] 连接异常断开\n", fd);
                close(fd);
                client_count--;
            }
        }
    }

    close(listen_fd);
    close(epfd);
    return 0;
}
