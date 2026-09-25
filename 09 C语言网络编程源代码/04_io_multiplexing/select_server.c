/*
 * Select模型IO多路复用Echo服务器
 * 原理：通过fd_set位图标记要监听的fd，select返回有事件的fd集合
 * 局限：1. FD_SETSIZE通常为1024，并发上限硬编码；2. 每次调用要轮询全部fd；3. 需要每次重新设置fd_set
 * 适用：跨平台兼容、并发连接数不大的场景
 * 编译：gcc -o select_server select_server.c ../common/common.c -I../common
 * 测试：开多个 nc 127.0.0.1 8892
 */
#include "common.h"

#define MAX_CLIENTS FD_SETSIZE

int main(int argc, char *argv[]) {
    int port = argc > 1 ? atoi(argv[1]) : 8892;
    signal(SIGPIPE, SIG_IGN);
    
    int listenfd = create_tcp_server("0.0.0.0", port, 128);
    
    fd_set allfds, readfds;
    FD_ZERO(&allfds);
    FD_SET(listenfd, &allfds);
    int maxfd = listenfd;
    
    char bufs[MAX_CLIENTS][4096]; // 每个客户端的读缓冲
    
    log_info("Select模型服务器启动，端口：%d, 最大并发%d", port, FD_SETSIZE);
    
    while (1) {
        readfds = allfds; // select会修改fd_set，每次必须重新拷贝
        int nready = select(maxfd + 1, &readfds, NULL, NULL, NULL);
        if (nready < 0) {
            if (errno == EINTR) continue;
            log_error("select");
            break;
        }
        
        // 检查监听套接字是否有新连接
        if (FD_ISSET(listenfd, &readfds)) {
            struct sockaddr_in cliaddr;
            socklen_t clilen = sizeof(cliaddr);
            int connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &clilen);
            if (connfd < 0) {
                log_error("accept");
            } else {
                if (connfd >= MAX_CLIENTS) {
                    log_warn("达到最大连接数%d, 拒绝连接", MAX_CLIENTS);
                    close(connfd);
                } else {
                    FD_SET(connfd, &allfds);
                    if (connfd > maxfd) maxfd = connfd;
                    char ip[INET_ADDRSTRLEN];
                    int p;
                    get_peer_addr(connfd, ip, &p);
                    log_info("新连接：%s:%d (fd=%d)", ip, p, connfd);
                }
            }
            if (--nready == 0) continue;
        }
        
        // 遍历所有客户端fd检查是否可读
        for (int fd = listenfd + 1; fd <= maxfd && nready > 0; fd++) {
            if (!FD_ISSET(fd, &allfds)) continue;
            if (!FD_ISSET(fd, &readfds)) continue;
            nready--;
            
            ssize_t n = read(fd, bufs[fd], sizeof(bufs[fd]));
            if (n > 0) {
                bufs[fd][n] = '\0';
                log_info("fd=%d: %s", fd, bufs[fd]);
                writen(fd, bufs[fd], n);
            } else if (n == 0) {
                // 客户端关闭
                log_info("fd=%d 断开", fd);
                close(fd);
                FD_CLR(fd, &allfds);
                // 更新maxfd（可选，简化起见不做）
            } else {
                log_error("read fd=%d", fd);
                close(fd);
                FD_CLR(fd, &allfds);
            }
        }
    }
    
    close(listenfd);
    return 0;
}
