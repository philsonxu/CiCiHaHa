#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>
#include <sys/stat.h>
#include <pthread.h>
#include <sys/epoll.h>
#include <sys/select.h>
#include <sys/poll.h>
#include <sys/uio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <ctype.h>

// 日志级别
#define LOG_LEVEL_DEBUG 0
#define LOG_LEVEL_INFO  1
#define LOG_LEVEL_WARN  2
#define LOG_LEVEL_ERROR 3

extern int g_log_level;

// 日志宏
#define log_debug(fmt, ...) \
    do { if (g_log_level <= LOG_LEVEL_DEBUG) \
        fprintf(stdout, "[DEBUG][%s:%d] " fmt "\n", __func__, __LINE__, ##__VA_ARGS__); } while(0)
#define log_info(fmt, ...) \
    do { if (g_log_level <= LOG_LEVEL_INFO) \
        fprintf(stdout, "[INFO] " fmt "\n", ##__VA_ARGS__); } while(0)
#define log_warn(fmt, ...) \
    do { if (g_log_level <= LOG_LEVEL_WARN) \
        fprintf(stdout, "[WARN] " fmt "\n", ##__VA_ARGS__); } while(0)
#define log_error(fmt, ...) \
    do { fprintf(stderr, "[ERROR][%s:%d] " fmt " : %s\n", __func__, __LINE__, ##__VA_ARGS__, strerror(errno)); } while(0)

#define ERR_EXIT(msg) \
    do { log_error(msg); exit(EXIT_FAILURE); } while(0)

// Socket通用封装
int create_tcp_server(const char *ip, int port, int backlog);
int create_tcp_client(const char *ip, int port);
int create_udp_socket();
int set_nonblocking(int fd);
int set_reuseaddr(int fd);
int set_reuseport(int fd);
int set_tcp_nodelay(int fd);
int set_keepalive(int fd, int idle, int interval, int count);
int set_send_buf(int fd, int size);
int set_recv_buf(int fd, int size);
int get_peer_addr(int fd, char *ip, int *port);
int get_sock_addr(int fd, char *ip, int *port);
ssize_t readn(int fd, void *buf, size_t count);
ssize_t writen(int fd, const void *buf, size_t count);
ssize_t readline(int fd, void *buf, size_t maxlen);
uint64_t get_timestamp_ms();
void hexdump(const void *data, size_t len);

// HTTP解析相关
typedef struct {
    char method[16];
    char path[1024];
    char version[16];
    char host[256];
    char content_type[256];
    long content_length;
    int keep_alive;
    char *body;
    size_t body_len;
} http_request_t;

int http_parse_request(const char *buf, size_t len, http_request_t *req);
void http_build_response(char *buf, size_t buf_len, int status, 
                         const char *content_type, const char *body, int keep_alive);

#endif // COMMON_H
