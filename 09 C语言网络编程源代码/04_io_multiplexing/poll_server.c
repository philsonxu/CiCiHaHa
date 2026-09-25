/*
 * Poll模型IO多路复用Echo服务器
 * 改进select的缺点：1. 没有1024硬限制（用数组动态扩容）；2. 不需要每次重置全部fd集合（events/revents分离）
 * 缺点：仍然需要每次轮询全部已注册fd检查revents，大并发下线性遍历开销大
 * 编译：gcc -o poll_server poll_server.c ../common/common.c -I../common
 * 测试：开多个 nc 127.0.0.1 8893
 */
#include "common.h"

#define INIT_SIZE 64

typedef struct {
    int fd;
    char buf[4096];
} client_t;

int main(int argc, char *argv[]) {
    int port = argc > 1 ? atoi(argv[1]) : 8893;
    signal(SIGPIPE, SIG_IGN);
    
    int listenfd = create_tcp_server("0.0.0.0", port, 128);
    
    int max_clients = INIT_SIZE;
    struct pollfd *pfds = malloc(sizeof(struct pollfd) * max_clients);
    client_t *clients = calloc(max_clients, sizeof(client_t));
    
    // pfds[0] 固定为监听套接字
    pfds[0].fd = listenfd;
    pfds[0].events = POLLIN;
    int nfds = 1;
    
    log_info("Poll模型服务器启动，端口：%d", port);
    
    while (1) {
        int nready = poll(pfds, nfds, -1);
        if (nready < 0) {
            if (errno == EINTR) continue;
            log_error("poll");
            break;
        }
        
        // 检查监听套接字
        if (pfds[0].revents & POLLIN) {
            struct sockaddr_in cliaddr;
            socklen_t clilen = sizeof(cliaddr);
            int connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &clilen);
            if (connfd >= 0) {
                // 动态扩容
                if (nfds == max_clients) {
                    max_clients *= 2;
                    pfds = realloc(pfds, sizeof(struct pollfd) * max_clients);
                    clients = realloc(clients, sizeof(client_t) * max_clients);
                    memset(&clients[nfds], 0, sizeof(client_t) * (max_clients - nfds));
                }
                pfds[nfds].fd = connfd;
                pfds[nfds].events = POLLIN;
                clients[nfds].fd = connfd;
                
                char ip[INET_ADDRSTRLEN];
                int p;
                get_peer_addr(connfd, ip, &p);
                log_info("新连接 fd=%d: %s:%d", connfd, ip, p);
                nfds++;
            }
            if (--nready == 0) continue;
        }
        
        // 遍历所有客户端
        for (int i = 1; i < nfds && nready > 0; i++) {
            if (pfds[i].revents == 0) continue;
            nready--;
            int fd = pfds[i].fd;
            
            if (pfds[i].revents & (POLLIN | POLLERR | POLLHUP)) {
                ssize_t n = read(fd, clients[i].buf, sizeof(clients[i].buf)-1);
                if (n > 0) {
                    clients[i].buf[n] = '\0';
                    log_info("fd=%d: %s", fd, clients[i].buf);
                    writen(fd, clients[i].buf, n);
                } else {
                    log_info("fd=%d 断开", fd);
                    close(fd);
                    // 与最后一个元素交换删除，避免数组移动
                    pfds[i] = pfds[nfds-1];
                    clients[i] = clients[nfds-1];
                    nfds--;
                    i--; // 退一步重新检查当前位置
                }
            }
        }
    }
    
    free(pfds);
    free(clients);
    close(listenfd);
    return 0;
}
