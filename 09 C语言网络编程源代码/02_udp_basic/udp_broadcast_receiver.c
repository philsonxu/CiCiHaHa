/*
 * UDP广播接收端
 * 编译：gcc -o udp_broadcast_receiver udp_broadcast_receiver.c ../common/common.c -I../common
 */
#include "common.h"

int main(int argc, char *argv[]) {
    int port = argc > 1 ? atoi(argv[1]) : 9999;
    
    int sockfd = create_udp_socket();
    set_reuseaddr(sockfd);
    
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        ERR_EXIT("bind");
    }
    
    log_info("广播接收端已启动，监听端口%d...", port);
    char buf[4096];
    while (1) {
        struct sockaddr_in from;
        socklen_t fromlen = sizeof(from);
        ssize_t n = recvfrom(sockfd, buf, sizeof(buf)-1, 0, (struct sockaddr*)&from, &fromlen);
        if (n > 0) {
            buf[n] = '\0';
            log_info("收到广播来自%s:%d: %s", inet_ntoa(from.sin_addr), ntohs(from.sin_port), buf);
        }
    }
    
    close(sockfd);
    return 0;
}
