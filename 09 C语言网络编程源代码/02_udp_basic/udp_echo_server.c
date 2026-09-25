/*
 * UDP Echo服务器
 * UDP特点：无连接、不保证可靠、不保证顺序、有消息边界
 * 与TCP区别：不需要listen/accept，直接recvfrom/sendto，一个socket可与多个客户端通信
 * 编译：gcc -o udp_echo_server udp_echo_server.c ../common/common.c -I../common
 * 测试：执行 ./udp_echo_client 127.0.0.1 8891 hello
 */
#include "common.h"

int main(int argc, char *argv[]) {
    int port = argc > 1 ? atoi(argv[1]) : 8891;
    
    int sockfd = create_udp_socket();
    
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);
    
    if (bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        ERR_EXIT("bind");
    }
    log_info("UDP Echo服务器启动，端口：%d", port);
    
    char buf[4096];
    while (1) {
        struct sockaddr_in cliaddr;
        socklen_t clilen = sizeof(cliaddr);
        ssize_t n = recvfrom(sockfd, buf, sizeof(buf)-1, 0, 
                             (struct sockaddr*)&cliaddr, &clilen);
        if (n < 0) {
            log_error("recvfrom");
            continue;
        }
        buf[n] = '\0';
        
        char ip[INET_ADDRSTRLEN];
        int cport;
        get_peer_addr(sockfd, ip, &cport); // 注意UDP用getpeername会报错，这里仅演示
        log_info("收到UDP数据来自%s:%d: %s", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port), buf);
        
        // 原样回送
        sendto(sockfd, buf, n, 0, (struct sockaddr*)&cliaddr, clilen);
    }
    
    close(sockfd);
    return 0;
}
