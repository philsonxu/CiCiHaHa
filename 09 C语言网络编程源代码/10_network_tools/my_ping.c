/*
 * 简易Ping工具（ICMP协议）
 * 原理：构造ICMP ECHO_REQUEST包发送，等待ICMP ECHO_REPLY，计算RTT
 * ICMP头部：类型(8bit)+代码(8bit)+校验和(16bit)+ID(16bit)+序号(16bit)+数据
 * 注意：需要root权限运行（原始套接字需要CAP_NET_RAW）
 * 编译：gcc -o my_ping my_ping.c ../common/common.c -I../common -lm
 * 运行：sudo ./my_ping www.baidu.com
 */
#include "common.h"
#include <netinet/ip_icmp.h>
#include <sys/time.h>

#define PACKET_SIZE 64
#define MAX_WAIT_TIME 1000

unsigned short checksum(void *b, int len) {
    unsigned short *buf = b;
    unsigned int sum = 0;
    for (sum = 0; len > 1; len -= 2) sum += *buf++;
    if (len == 1) sum += *(unsigned char*)buf;
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    return ~sum;
}

long long time_diff_ms(struct timespec *start, struct timespec *end) {
    return (end->tv_sec - start->tv_sec) * 1000LL + (end->tv_nsec - start->tv_nsec) / 1000000LL;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("用法: sudo %s <域名/IP>\n", argv[0]);
        return 1;
    }
    
    // 解析地址
    struct sockaddr_in dest;
    memset(&dest, 0, sizeof(dest));
    dest.sin_family = AF_INET;
    
    struct hostent *host = gethostbyname(argv[1]);
    if (!host) {
        // 直接当作IP解析
        if (inet_pton(AF_INET, argv[1], &dest.sin_addr) != 1) {
            log_error("无法解析主机名");
            return 1;
        }
    } else {
        memcpy(&dest.sin_addr, host->h_addr, host->h_length);
    }
    
    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &dest.sin_addr, ip, sizeof(ip));
    log_info("PING %s (%s): %d data bytes", argv[1], ip, PACKET_SIZE);
    
    // 创建ICMP原始套接字
    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sockfd < 0) {
        log_error("socket原始套接字创建失败，请使用sudo运行");
        return 1;
    }
    
    struct timeval tv = {1, 0};
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    
    pid_t pid = getpid() & 0xFFFF;
    int seq = 0;
    int sent = 0, received = 0;
    long long total_rtt = 0, min_rtt = 999999, max_rtt = 0;
    
    for (int i = 0; i < 4; i++) {
        // 构造ICMP包
        char sendbuf[PACKET_SIZE];
        memset(sendbuf, 0, sizeof(sendbuf));
        struct icmphdr *icmp = (struct icmphdr*)sendbuf;
        icmp->type = ICMP_ECHO;
        icmp->code = 0;
        icmp->un.echo.id = htons(pid);
        icmp->un.echo.sequence = htons(seq++);
        // 时间戳放在数据区
        struct timespec send_time;
        clock_gettime(CLOCK_MONOTONIC, &send_time);
        memcpy(sendbuf + sizeof(struct icmphdr), &send_time, sizeof(send_time));
        icmp->checksum = 0;
        icmp->checksum = checksum(sendbuf, PACKET_SIZE);
        
        sent++;
        if (sendto(sockfd, sendbuf, PACKET_SIZE, 0, (struct sockaddr*)&dest, sizeof(dest)) < 0) {
            log_error("sendto");
            continue;
        }
        
        // 接收回复
        char recvbuf[1024];
        struct sockaddr_in from;
        socklen_t fromlen = sizeof(from);
        ssize_t n = recvfrom(sockfd, recvbuf, sizeof(recvbuf), 0, (struct sockaddr*)&from, &fromlen);
        if (n > 0) {
            struct timespec recv_time;
            clock_gettime(CLOCK_MONOTONIC, &recv_time);
            
            // IP头偏移（20字节）跳过到ICMP头
            struct icmphdr *ricmp = (struct icmphdr*)(recvbuf + 20);
            if (ricmp->type == ICMP_ECHOREPLY && ntohs(ricmp->un.echo.id) == pid) {
                received++;
                long long rtt = time_diff_ms(&send_time, &recv_time);
                total_rtt += rtt;
                if (rtt < min_rtt) min_rtt = rtt;
                if (rtt > max_rtt) max_rtt = rtt;
                log_info("%zd bytes from %s: icmp_seq=%d ttl=64 time=%lld ms",
                        n - 20, ip, i+1, rtt);
            }
        } else {
            log_warn("请求超时 icmp_seq=%d", i+1);
        }
        sleep(1);
    }
    
    // 统计
    log_info("--- %s ping统计 ---", argv[1]);
    log_info("%d packets transmitted, %d received, %d%% packet loss",
             sent, received, (sent - received) * 100 / sent);
    if (received > 0) {
        log_info("rtt min/avg/max = %lld/%lld/%lld ms",
                 min_rtt, total_rtt / received, max_rtt);
    }
    
    close(sockfd);
    return 0;
}
