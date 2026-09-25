/*
 * 可靠UDP(RUDP)简单实现
 * 在UDP基础上实现TCP的核心可靠机制：
 *   1. 序号Sequence：保证数据有序
 *   2. 确认应答ACK：接收方告诉发送方收到了哪些数据
 *   3. 超时重传：发送方一段时间没收到ACK就重发
 * 这就是KCP/QUIC等可靠UDP协议的最基本原理
 * 本Demo实现简单Stop-and-Wait(停等协议)，便于理解核心思想
 * 编译：gcc -o rudp_demo rudp_echo_demo.c ../common/common.c -I../common
 * 测试：先启动服务端 ./rudp_demo server ，再启动客户端 ./rudp_demo client
 */
#include "common.h"

#define RUDP_PORT 8899
#define RUDP_TIMEOUT_MS 1000
#define RUDP_MAX_RETRY 5
#define RUDP_BUF_SIZE 1024

// RUDP包类型
#define RUDP_ACK  0
#define RUDP_DATA 1
#define RUDP_FIN  2

typedef struct __attribute__((packed)) {
    uint8_t type;
    uint32_t seq;     // 序号
    uint32_t ack;     // 确认号
    uint16_t len;     // 数据长度
    char data[RUDP_BUF_SIZE];
} rudp_pkt_t;

int rudp_send(int fd, struct sockaddr_in *addr, rudp_pkt_t *pkt) {
    socklen_t alen = sizeof(struct sockaddr_in);
    return sendto(fd, pkt, sizeof(rudp_pkt_t) - RUDP_BUF_SIZE + pkt->len, 0,
                  (struct sockaddr*)addr, alen);
}

int rudp_recv(int fd, rudp_pkt_t *pkt, struct sockaddr_in *addr) {
    socklen_t alen = sizeof(struct sockaddr_in);
    struct timeval tv = {0, 10000}; // 10ms非阻塞轮询
    setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    ssize_t n = recvfrom(fd, pkt, sizeof(rudp_pkt_t), 0, (struct sockaddr*)addr, &alen);
    return n;
}

// 可靠发送：等待ACK，超时重传
int rudp_send_reliable(int fd, struct sockaddr_in *peer, rudp_pkt_t *pkt) {
    for (int retry = 0; retry < RUDP_MAX_RETRY; retry++) {
        rudp_send(fd, peer, pkt);
        uint64_t start = get_timestamp_ms();
        
        while (get_timestamp_ms() - start < RUDP_TIMEOUT_MS) {
            rudp_pkt_t resp;
            struct sockaddr_in from;
            int n = rudp_recv(fd, &resp, &from);
            if (n > 0 && resp.type == RUDP_ACK && resp.ack == pkt->seq) {
                return 0; // ACK确认
            }
        }
        log_warn("seq=%u 超时，重传(第%d次)", pkt->seq, retry+1);
    }
    return -1;
}

void run_server() {
    int fd = create_udp_socket();
    set_reuseaddr(fd);
    
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(RUDP_PORT);
    bind(fd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    
    log_info("RUDP服务器启动，端口：%d", RUDP_PORT);
    
    struct sockaddr_in client;
    uint32_t expected_seq = 0;
    
    while (1) {
        rudp_pkt_t pkt;
        memset(&pkt, 0, sizeof(pkt));
        int n = rudp_recv(fd, &pkt, &client);
        if (n <= 0) continue;
        
        if (pkt.type == RUDP_DATA) {
            log_info("收到DATA seq=%u len=%u", pkt.seq, pkt.len);
            
            if (pkt.seq == expected_seq) {
                // 按序收到，投递应用层，期望序号+1
                pkt.data[pkt.len] = '\0';
                log_info("  内容：%s", pkt.data);
                expected_seq++;
            } else {
                log_warn("  乱序/重复，期望seq=%u 收到%u，丢弃", expected_seq, pkt.seq);
            }
            
            // 不管是否按序，都回ACK（累计确认，确认号=expected_seq-1）
            rudp_pkt_t ack;
            memset(&ack, 0, sizeof(ack));
            ack.type = RUDP_ACK;
            ack.ack = pkt.seq;
            rudp_send(fd, &client, &ack);
        }
        else if (pkt.type == RUDP_FIN) {
            log_info("收到FIN，客户端关闭连接");
            rudp_pkt_t ack;
            memset(&ack, 0, sizeof(ack));
            ack.type = RUDP_ACK;
            ack.ack = pkt.seq;
            rudp_send(fd, &client, &ack);
            expected_seq = 0; // 重置，等待新客户端
        }
    }
    close(fd);
}

void run_client() {
    int fd = create_udp_socket();
    
    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(RUDP_PORT);
    inet_pton(AF_INET, "127.0.0.1", &server.sin_addr);
    
    log_info("RUDP客户端启动，目标端口：%d", RUDP_PORT);
    
    const char *messages[] = {
        "Hello RUDP!",
        "This is message #2",
        "Reliable UDP demo",
        "Number 4 packet",
        "The last message"
    };
    int count = sizeof(messages)/sizeof(messages[0]);
    
    uint32_t seq = 0;
    for (int i = 0; i < count; i++) {
        rudp_pkt_t pkt;
        memset(&pkt, 0, sizeof(pkt));
        pkt.type = RUDP_DATA;
        pkt.seq = seq++;
        pkt.len = strlen(messages[i]);
        memcpy(pkt.data, messages[i], pkt.len);
        
        log_info("发送seq=%u: %s", pkt.seq, messages[i]);
        if (rudp_send_reliable(fd, &server, &pkt) < 0) {
            log_error("发送失败，重传次数耗尽");
            break;
        }
        log_info("发送成功，收到ACK");
        usleep(200000); // 模拟间隔
    }
    
    // 发送FIN
    rudp_pkt_t fin;
    memset(&fin, 0, sizeof(fin));
    fin.type = RUDP_FIN;
    fin.seq = seq;
    rudp_send_reliable(fd, &server, &fin);
    log_info("===== 所有消息发送完成 =====");
    
    close(fd);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("用法: %s server|client\n", argv[0]);
        return 1;
    }
    signal(SIGPIPE, SIG_IGN);
    
    if (strcmp(argv[1], "server") == 0) {
        run_server();
    } else if (strcmp(argv[1], "client") == 0) {
        run_client();
    } else {
        printf("未知模式: %s\n", argv[1]);
        return 1;
    }
    return 0;
}
