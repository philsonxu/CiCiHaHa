/*
 * Socket常用选项演示
 * 包含：SO_REUSEADDR/SO_REUSEPORT、SO_SNDBUF/SO_RCVBUF、SO_RCVTIMEO/SO_SNDTIMEO、
 *       SO_LINGER、TCP_NODELAY（Nagle算法开关）、SO_KEEPALIVE
 * 编译：gcc -o socket_options_demo socket_options_demo.c ../common/common.c -I../common
 */
#include "common.h"
#include <sys/wait.h>

void print_buf_size(int fd) {
    int snd, rcv;
    socklen_t len = sizeof(int);
    getsockopt(fd, SOL_SOCKET, SO_SNDBUF, &snd, &len);
    getsockopt(fd, SOL_SOCKET, SO_RCVBUF, &rcv, &len);
    log_info("发送缓冲区大小：%d bytes, 接收缓冲区大小：%d bytes", snd, rcv);
}

int main() {
    signal(SIGPIPE, SIG_IGN);
    
    log_info("===== Socket选项演示 =====");
    
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    
    // 1. 查询默认缓冲区大小
    log_info("--- 默认缓冲区大小 ---");
    print_buf_size(fd);
    
    // 2. 设置并查询缓冲区大小
    log_info("--- 设置发送缓冲区64K, 接收缓冲区64K ---");
    set_send_buf(fd, 65536);
    set_recv_buf(fd, 65536);
    print_buf_size(fd);
    log_info("注意：内核会将设置值翻倍（预留开销），这是正常现象");
    
    // 3. 设置接收超时
    log_info("--- 设置接收超时3秒测试 ---");
    struct timeval tv = {3, 0};
    setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    char buf[10];
    ssize_t n = read(fd, buf, sizeof(buf));
    if (n < 0 && errno == EAGAIN) {
        log_info("read在3秒后超时返回EAGAIN/EWOULDBLOCK，符合预期");
    }
    
    // 4. TCP_NODELAY 禁用Nagle算法
    log_info("--- TCP_NODELAY (禁用Nagle算法, 小包立即发送) ---");
    int nodelay;
    socklen_t len = sizeof(nodelay);
    getsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &nodelay, &len);
    log_info("TCP_NODELAY默认值：%d (%s)", nodelay, nodelay ? "开启" : "关闭");
    set_tcp_nodelay(fd);
    getsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &nodelay, &len);
    log_info("设置后TCP_NODELAY值：%d", nodelay);
    
    // 5. SO_REUSEADDR 地址重用
    log_info("--- SO_REUSEADDR ---");
    int reuse;
    getsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuse, &len);
    log_info("SO_REUSEADDR默认值：%d", reuse);
    log_info("作用：服务器重启时允许立即绑定TIME_WAIT状态的端口，避免\"Address already in use\"");
    
    // 6. SO_LINGER 优雅关闭控制
    log_info("--- SO_LINGER ---");
    struct linger ling;
    getsockopt(fd, SOL_SOCKET, SO_LINGER, &ling, &len);
    log_info("默认l_onoff=%d, l_linger=%d (close立即返回，内核尝试发送剩余数据)",
             ling.l_onoff, ling.l_linger);
    log_info("l_onoff=1,l_linger=0: close立即返回并发送RST强制断开，跳过TIME_WAIT");
    log_info("l_onoff=1,l_linger=N: close阻塞最多N秒等待数据发送完和对端确认");
    
    // 7. SO_KEEPALIVE TCP保活
    log_info("--- SO_KEEPALIVE 保活机制 ---");
    int keepalive;
    getsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &keepalive, &len);
    log_info("SO_KEEPALIVE默认：%d (默认关闭)", keepalive);
    log_info("开启后，空闲2小时内核会自动发探测包检测连接存活");
    log_info("可通过TCP_KEEPIDLE/TCP_KEEPINTVL/TCP_KEEPCNT调整探测间隔");
    
    // 8. 获取连接本地/对端地址演示
    close(fd);
    int listenfd = create_tcp_server("127.0.0.1", 0, 5);
    char ip[INET_ADDRSTRLEN];
    int port;
    get_sock_addr(listenfd, ip, &port);
    log_info("--- 动态绑定端口演示 ---");
    log_info("监听套接字绑定在 %s:%d (端口0由内核自动分配)", ip, port);
    
    // 测试accept后获取对端地址
    int client = fork();
    if (client == 0) {
        close(listenfd);
        int cfd = create_tcp_client("127.0.0.1", port);
        sleep(1);
        close(cfd);
        exit(0);
    }
    
    struct sockaddr_in cliaddr;
    socklen_t clilen = sizeof(cliaddr);
    int connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &clilen);
    char cip[INET_ADDRSTRLEN];
    int cport;
    get_peer_addr(connfd, cip, &cport);
    log_info("accept返回对端地址：%s:%d", cip, cport);
    
    close(connfd);
    close(listenfd);
    wait(NULL);
    
    log_info("===== Socket选项演示完成 =====");
    return 0;
}
