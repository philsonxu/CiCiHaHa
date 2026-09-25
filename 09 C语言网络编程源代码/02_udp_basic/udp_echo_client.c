/*
 * UDP Echo客户端
 * 注意：UDP客户端不需要connect，直接sendto即可
 * 也可以调用connect，之后可用send/recv，但只是绑定目标地址，不建立连接
 * 编译：gcc -o udp_echo_client udp_echo_client.c ../common/common.c -I../common
 */
#include "common.h"

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("用法: %s <IP> <端口> <消息...>\n", argv[0]);
        return 1;
    }
    const char *ip = argv[1];
    int port = atoi(argv[2]);
    
    int sockfd = create_udp_socket();
    
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &servaddr.sin_addr);
    
    // 拼接所有命令行参数作为消息
    char msg[4096] = {0};
    for (int i = 3; i < argc; i++) {
        strcat(msg, argv[i]);
        if (i < argc - 1) strcat(msg, " ");
    }
    
    log_info("发送UDP消息到%s:%d: %s", ip, port, msg);
    sendto(sockfd, msg, strlen(msg), 0, (struct sockaddr*)&servaddr, sizeof(servaddr));
    
    // 等待回显
    char buf[4096];
    struct sockaddr_in recvaddr;
    socklen_t recvlen = sizeof(recvaddr);
    struct timeval tv = {3, 0};
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    
    ssize_t n = recvfrom(sockfd, buf, sizeof(buf)-1, 0, (struct sockaddr*)&recvaddr, &recvlen);
    if (n > 0) {
        buf[n] = '\0';
        log_info("收到回显: %s", buf);
    } else {
        log_error("接收超时");
    }
    
    close(sockfd);
    return 0;
}
