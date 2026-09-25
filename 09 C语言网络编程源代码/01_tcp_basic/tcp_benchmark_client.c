/*
 * TCP性能压测客户端
 * 功能：建立多个并发连接，每个连接循环发送/接收数据，统计QPS与延迟
 * 用法：./tcp_benchmark_client 127.0.0.1 8890 100 10000
 *       参数：IP 端口 并发连接数 每个连接请求数
 * 编译：gcc -o tcp_benchmark_client tcp_benchmark_client.c ../common/common.c -I../common -lpthread
 */
#include "common.h"

typedef struct {
    const char *ip;
    int port;
    int requests_per_conn;
    long long success;
    long long failed;
} thread_arg_t;

const char *REQUEST = "PING from benchmark client\n";

void* worker(void *arg) {
    thread_arg_t *t = (thread_arg_t*)arg;
    int req_len = strlen(REQUEST);
    char buf[1024];
    
    int sockfd = create_tcp_client(t->ip, t->port);
    if (sockfd < 0) { t->failed = t->requests_per_conn; return NULL; }
    
    for (int i = 0; i < t->requests_per_conn; i++) {
        if (writen(sockfd, REQUEST, req_len) != req_len) {
            t->failed++;
            break;
        }
        ssize_t n = readn(sockfd, buf, req_len);
        if (n == req_len) {
            t->success++;
        } else {
            t->failed++;
            break;
        }
    }
    close(sockfd);
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc < 5) {
        printf("用法: %s <IP> <端口> <并发连接数> <每连接请求数>\n", argv[0]);
        return 1;
    }
    const char *ip = argv[1];
    int port = atoi(argv[2]);
    int connections = atoi(argv[3]);
    int req_per_conn = atoi(argv[4]);
    
    log_info("压测参数：目标%s:%d，并发%d，每连接%d请求", ip, port, connections, req_per_conn);
    
    pthread_t *tids = malloc(sizeof(pthread_t) * connections);
    thread_arg_t *args = malloc(sizeof(thread_arg_t) * connections);
    
    uint64_t start = get_timestamp_ms();
    for (int i = 0; i < connections; i++) {
        args[i].ip = ip;
        args[i].port = port;
        args[i].requests_per_conn = req_per_conn;
        args[i].success = 0;
        args[i].failed = 0;
        pthread_create(&tids[i], NULL, worker, &args[i]);
    }
    
    long long total_success = 0, total_failed = 0;
    for (int i = 0; i < connections; i++) {
        pthread_join(tids[i], NULL);
        total_success += args[i].success;
        total_failed += args[i].failed;
    }
    uint64_t cost = get_timestamp_ms() - start;
    
    log_info("===== 压测结果 =====");
    log_info("总成功请求：%lld", total_success);
    log_info("总失败请求：%lld", total_failed);
    log_info("耗时：%llu ms", cost);
    if (cost > 0) {
        log_info("QPS：%.2f req/s", total_success * 1000.0 / cost);
        log_info("平均延迟：%.2f ms/req", cost * 1.0 / total_success);
    }
    
    free(tids);
    free(args);
    return 0;
}
