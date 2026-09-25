/*
 * 简易Traceroute路由追踪工具
 * 原理：发送IP包时TTL从1开始递增，每经过一个路由器TTL减1，TTL=0时路由器发回ICMP超时
 *       当到达目标主机时会回ICMP端口不可达，说明到达终点
 * TTL (Time To Live)：IP头的生存时间，每经过一跳减1，减到0丢弃
 * 编译：gcc -o my_traceroute my_traceroute.c ../common/common.c -I../common
 * 运行：sudo ./my_traceroute www.baidu.com
 */
#include "common.h"
#include <netinet/ip_icmp.h>
#include <netinet/udp.h>

#define MAX_HOPS 30
#define PROBES_PER_HOP 3
#define RECV_TIMEOUT 2

unsigned short checksum(void *b, int len) {
    unsigned short *buf = b;
    unsigned int sum = 0;
    for (sum = 0; len > 1; len -= 2) sum += *buf++;
    if (len == 1) sum += *(unsigned char*)buf;
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    return ~sum;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("用法: sudo %s <域名/IP>\n", argv[0]);
        return 1;
    }
    
    struct sockaddr_in dest;
    memset(&dest, 0, sizeof(dest));
    dest.sin_family = AF_INET;
    
    struct hostent *host = gethostbyname(argv[1]);
    if (!host) {
        if (inet_pton(AF_INET, argv[1], &dest.sin_addr) != 1) {
            log_error("无法解析主机");
            return 1;
        }
    } else {
        memcpy(&dest.sin_addr, host->h_addr, host->h_length);
    }
    
    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &dest.sin_addr, ip, sizeof(ip));
    log_info("traceroute to %s (%s), %d hops max", argv[1], ip, MAX_HOPS);
    
    // 发送用UDP端口（高端口，目标端口不可能开放）
    int sendfd = socket(AF_INET, SOCK_DGRAM, 0);
    // 接收ICMP用原始套接字
    int recvfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (recvfd < 0) {
        log_error("创建ICMP套接字失败，请用sudo运行");
        return 1;
    }
    
    struct timeval tv = {RECV_TIMEOUT, 0};
    setsockopt(recvfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    
    int dest_port = 33434;
    bool reached = false;
    
    for (int ttl = 1; ttl <= MAX_HOPS && !reached; ttl++) {
        // 设置IP TTL
        setsockopt(sendfd, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl));
        
        printf("%2d  ", ttl);
        fflush(stdout);
        
        char last_ip[INET_ADDRSTRLEN] = "";
        
        for (int probe = 0; probe < PROBES_PER_HOP; probe++) {
            struct sockaddr_in probe_addr = dest;
            probe_addr.sin_port = htons(dest_port + ttl);
            
            uint64_t start = get_timestamp_ms();
            char buf[1] = {0};
            sendto(sendfd, buf, 1, 0, (struct sockaddr*)&probe_addr, sizeof(probe_addr));
            
            char recvbuf[1024];
            struct sockaddr_in from;
            socklen_t fromlen = sizeof(from);
            ssize_t n = recvfrom(recvfd, recvbuf, sizeof(recvbuf), 0, (struct sockaddr*)&from, &fromlen);
            
            if (n > 0) {
                uint64_t rtt = get_timestamp_ms() - start;
                char rip[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, &from.sin_addr, rip, sizeof(rip));
                
                // 判断是否是ICMP超时（TTL=0）或端口不可达（到达目标）
                struct icmphdr *icmp = (struct icmphdr*)(recvbuf + 20);
                if (icmp->type == ICMP_TIME_EXCEEDED) {
                    printf(" %s (%llu ms)", rip, rtt);
                } else if (icmp->type == ICMP_DEST_UNREACH && icmp->code == ICMP_PORT_UNREACH) {
                    printf(" %s (%llu ms)", rip, rtt);
                    reached = true;
                } else {
                    printf(" *");
                }
                
                if (probe == PROBES_PER_HOP - 1 && from.sin_addr.s_addr == dest.sin_addr.s_addr) {
                    reached = true;
                }
                strncpy(last_ip, rip, sizeof(last_ip)-1);
            } else {
                printf(" *");
            }
            fflush(stdout);
        }
        printf("\n");
    }
    
    close(sendfd);
    close(recvfd);
    return 0;
}
