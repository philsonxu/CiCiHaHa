/*
 * UDP广播发送端
 * 原理：设置SO_BROADCAST选项后，发送到255.255.255.255或子网广播地址
 * 同网段内所有监听该端口的主机都能收到消息
 * 应用场景：局域网设备发现、局域网通知
 * 编译：gcc -o udp_broadcast_sender udp_broadcast_sender.c ../common/common.c -I../common
 * 测试：先在另一终端启动udp_broadcast_receiver，再运行本程序发送广播
 */
#include "common.h"

int main(int argc, char *argv[]) {
    int port = argc > 1 ? atoi(argv[1]) : 9999;
    
    int sockfd = create_udp_socket();
    
    // 关键：开启广播权限
    int broadcast = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) < 0) {
        ERR_EXIT("setsockopt SO_BROADCAST");
    }
    
    struct sockaddr_in broadaddr;
    memset(&broadaddr, 0, sizeof(broadaddr));
    broadaddr.sin_family = AF_INET;
    broadaddr.sin_port = htons(port);
    broadaddr.sin_addr.s_addr = htonl(INADDR_BROADCAST); // 255.255.255.255
    
    log_info("开始每2秒发送广播消息到端口%d...", port);
    int seq = 0;
    while (1) {
        char msg[256];
        snprintf(msg, sizeof(msg), "Broadcast message #%d from PID %d", seq++, getpid());
        sendto(sockfd, msg, strlen(msg), 0, (struct sockaddr*)&broadaddr, sizeof(broadaddr));
        log_info("已发送: %s", msg);
        sleep(2);
    }
    
    close(sockfd);
    return 0;
}
