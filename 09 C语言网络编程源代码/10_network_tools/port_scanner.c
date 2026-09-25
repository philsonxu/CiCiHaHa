/*
 * TCP端口扫描器
 * 原理：非阻塞connect + select/poll实现快速多端口扫描
 * connect返回EINPROGRESS表示正在连接，用select等待可写事件
 * select返回后用getsockopt SO_ERROR判断是否连接成功
 * 这是nmap SYN扫描之外最常用的TCP Connect扫描技术
 * 编译：gcc -o port_scanner port_scanner.c ../common/common.c -I../common
 * 运行：./port_scanner 127.0.0.1 1 1024  扫描1-1024端口
 */
#include "common.h"

typedef struct {
    int port;
    int fd;
} scan_job_t;

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("用法: %s <IP> <起始端口> <结束端口>\n", argv[0]);
        printf("示例: %s 127.0.0.1 1 1024\n", argv[0]);
        return 1;
    }
    
    const char *target_ip = argv[1];
    int start_port = atoi(argv[2]);
    int end_port = atoi(argv[3]);
    
    if (start_port > end_port || start_port < 1 || end_port > 65535) {
        log_error("端口范围无效");
        return 1;
    }
    
    int total = end_port - start_port + 1;
    log_info("开始扫描 %s 端口 %d - %d (共%d个端口)", target_ip, start_port, end_port, total);
    uint64_t start_time = get_timestamp_ms();
    
    int open_count = 0;
    int pending = 0;
    const int MAX_PARALLEL = 100; // 同时并发100个连接，可调整
    
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    inet_pton(AF_INET, target_ip, &addr.sin_addr);
    
    scan_job_t jobs[65536];
    int job_count = 0;
    int current_port = start_port;
    
    fd_set wfds;
    FD_ZERO(&wfds);
    int maxfd = 0;
    
    // 先发第一批连接
    while (current_port <= end_port && pending < MAX_PARALLEL) {
        int fd = socket(AF_INET, SOCK_STREAM, 0);
        set_nonblocking(fd);
        addr.sin_port = htons(current_port);
        
        int ret = connect(fd, (struct sockaddr*)&addr, sizeof(addr));
        if (ret == 0) {
            // 立即连接成功（罕见，本地回环）
            log_info("端口 %d : OPEN (immediate)", current_port);
            open_count++;
            close(fd);
        } else if (errno == EINPROGRESS) {
            jobs[job_count].fd = fd;
            jobs[job_count].port = current_port;
            FD_SET(fd, &wfds);
            if (fd > maxfd) maxfd = fd;
            job_count++;
            pending++;
        } else {
            close(fd);
        }
        current_port++;
    }
    
    // 事件循环等待连接结果
    while (pending > 0) {
        fd_set tmpfds = wfds;
        struct timeval tv = {2, 0};
        int n = select(maxfd + 1, NULL, &tmpfds, NULL, &tv);
        
        for (int i = 0; i < job_count; i++) {
            if (jobs[i].fd < 0) continue;
            if (!FD_ISSET(jobs[i].fd, &tmpfds)) continue;
            
            int err = 0;
            socklen_t len = sizeof(err);
            getsockopt(jobs[i].fd, SOL_SOCKET, SO_ERROR, &err, &len);
            
            if (err == 0) {
                log_info("端口 %d : OPEN", jobs[i].port);
                open_count++;
            }
            // 关闭已完成的socket
            close(jobs[i].fd);
            FD_CLR(jobs[i].fd, &wfds);
            jobs[i].fd = -1;
            pending--;
        }
        
        // 补充新端口继续扫描
        while (current_port <= end_port && pending < MAX_PARALLEL) {
            int fd = socket(AF_INET, SOCK_STREAM, 0);
            set_nonblocking(fd);
            addr.sin_port = htons(current_port);
            
            int ret = connect(fd, (struct sockaddr*)&addr, sizeof(addr));
            if (ret == 0) {
                log_info("端口 %d : OPEN", current_port);
                open_count++;
                close(fd);
            } else if (errno == EINPROGRESS) {
                // 找一个空闲槽位
                int slot = -1;
                for (int j = 0; j < job_count; j++) {
                    if (jobs[j].fd < 0) { slot = j; break; }
                }
                if (slot == -1) slot = job_count++;
                jobs[slot].fd = fd;
                jobs[slot].port = current_port;
                FD_SET(fd, &wfds);
                if (fd > maxfd) maxfd = fd;
                pending++;
            } else {
                close(fd);
            }
            current_port++;
        }
        
        // 更新maxfd
        maxfd = 0;
        for (int j = 0; j < job_count; j++) {
            if (jobs[j].fd >= 0 && jobs[j].fd > maxfd) maxfd = jobs[j].fd;
        }
    }
    
    uint64_t cost = get_timestamp_ms() - start_time;
    log_info("===== 扫描完成 =====");
    log_info("开放端口数：%d", open_count);
    log_info("耗时：%llu ms", cost);
    log_info("扫描速度：%.1f 端口/秒", total * 1000.0 / cost);
    
    return 0;
}
