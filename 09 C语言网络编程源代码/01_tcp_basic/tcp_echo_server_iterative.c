/*
 * 迭代式TCP Echo服务器（基础模型）
 * 原理：单进程循环accept，每次服务一个客户端，服务完才能处理下一个
 * 特点：代码简单，但同一时间只能处理一个客户端，是最基础的服务器模型
 * 编译：gcc -o tcp_echo_iterative tcp_echo_server_iterative.c ../common/common.c -I../common
 * 测试：在另一个终端执行 nc 127.0.0.1 8888 输入文字即可收到回显
 */
#include "common.h"

int main(int argc, char *argv[]) {
    int port = argc > 1 ? atoi(argv[1]) : 8888;
    
    signal(SIGPIPE, SIG_IGN); // 忽略向已关闭socket写的信号
    
    int listenfd = create_tcp_server("0.0.0.0", port, 5);
    log_info("迭代式Echo服务器启动，端口：%d", port);
    
    char buf[4096];
    while (1) {
        struct sockaddr_in cliaddr;
        socklen_t clilen = sizeof(cliaddr);
        int connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &clilen);
        if (connfd < 0) {
            if (errno == EINTR) continue;
            log_error("accept");
            continue;
        }
        
        char ip[INET_ADDRSTRLEN];
        int cport;
        get_peer_addr(connfd, ip, &cport);
        log_info("新客户端连接：%s:%d", ip, cport);
        
        // 循环读直到客户端关闭
        while (1) {
            ssize_t n = read(connfd, buf, sizeof(buf)-1);
            if (n > 0) {
                buf[n] = '\0';
                log_info("收到数据：%s", buf);
                writen(connfd, buf, n); // 原样回显
            } else if (n == 0) {
                log_info("客户端断开：%s:%d", ip, cport);
                break;
            } else {
                if (errno == EINTR) continue;
                log_error("read");
                break;
            }
        }
        close(connfd);
    }
    
    close(listenfd);
    return 0;
}
