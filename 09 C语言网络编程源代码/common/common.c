#include "common.h"

int g_log_level = LOG_LEVEL_INFO;

int set_reuseaddr(int fd) {
    int opt = 1;
    return setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
}

int set_reuseport(int fd) {
#ifdef SO_REUSEPORT
    int opt = 1;
    return setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));
#else
    return 0;
#endif
}

int set_nonblocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) return -1;
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

int set_tcp_nodelay(int fd) {
    int opt = 1;
    return setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &opt, sizeof(opt));
}

int set_keepalive(int fd, int idle, int interval, int count) {
    int opt = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &opt, sizeof(opt)) < 0) return -1;
#ifdef TCP_KEEPIDLE
    setsockopt(fd, IPPROTO_TCP, TCP_KEEPIDLE, &idle, sizeof(idle));
#endif
#ifdef TCP_KEEPINTVL
    setsockopt(fd, IPPROTO_TCP, TCP_KEEPINTVL, &interval, sizeof(interval));
#endif
#ifdef TCP_KEEPCNT
    setsockopt(fd, IPPROTO_TCP, TCP_KEEPCNT, &count, sizeof(count));
#endif
    return 0;
}

int set_send_buf(int fd, int size) {
    return setsockopt(fd, SOL_SOCKET, SO_SNDBUF, &size, sizeof(size));
}

int set_recv_buf(int fd, int size) {
    return setsockopt(fd, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size));
}

int create_tcp_server(const char *ip, int port, int backlog) {
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0) ERR_EXIT("socket");
    
    set_reuseaddr(listenfd);
    set_reuseport(listenfd);
    
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    if (ip == NULL || strcmp(ip, "0.0.0.0") == 0) {
        servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    } else {
        inet_pton(AF_INET, ip, &servaddr.sin_addr);
    }
    
    if (bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        close(listenfd);
        ERR_EXIT("bind");
    }
    
    if (listen(listenfd, backlog) < 0) {
        close(listenfd);
        ERR_EXIT("listen");
    }
    
    log_info("TCP server listening on %s:%d", ip ? ip : "0.0.0.0", port);
    return listenfd;
}

int create_tcp_client(const char *ip, int port) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) ERR_EXIT("socket");
    
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &servaddr.sin_addr);
    
    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        close(sockfd);
        ERR_EXIT("connect");
    }
    
    log_info("TCP connected to %s:%d", ip, port);
    return sockfd;
}

int create_udp_socket() {
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0) ERR_EXIT("socket");
    return fd;
}

int get_peer_addr(int fd, char *ip, int *port) {
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    if (getpeername(fd, (struct sockaddr*)&addr, &len) < 0) return -1;
    inet_ntop(AF_INET, &addr.sin_addr, ip, INET_ADDRSTRLEN);
    *port = ntohs(addr.sin_port);
    return 0;
}

int get_sock_addr(int fd, char *ip, int *port) {
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    if (getsockname(fd, (struct sockaddr*)&addr, &len) < 0) return -1;
    inet_ntop(AF_INET, &addr.sin_addr, ip, INET_ADDRSTRLEN);
    *port = ntohs(addr.sin_port);
    return 0;
}

// 可靠读n字节
ssize_t readn(int fd, void *buf, size_t count) {
    size_t nleft = count;
    ssize_t nread;
    char *ptr = (char*)buf;
    while (nleft > 0) {
        if ((nread = read(fd, ptr, nleft)) < 0) {
            if (errno == EINTR) continue;
            return -1;
        } else if (nread == 0) break;
        nleft -= nread;
        ptr += nread;
    }
    return count - nleft;
}

// 可靠写n字节
ssize_t writen(int fd, const void *buf, size_t count) {
    size_t nleft = count;
    ssize_t nwritten;
    const char *ptr = (const char*)buf;
    while (nleft > 0) {
        if ((nwritten = write(fd, ptr, nleft)) < 0) {
            if (errno == EINTR) continue;
            return -1;
        } else if (nwritten == 0) break;
        nleft -= nwritten;
        ptr += nwritten;
    }
    return count - nleft;
}

// 按行读取
ssize_t readline(int fd, void *buf, size_t maxlen) {
    ssize_t n, rc;
    char c, *ptr = (char*)buf;
    for (n = 1; n < maxlen; n++) {
again:
        if ((rc = read(fd, &c, 1)) == 1) {
            *ptr++ = c;
            if (c == '\n') break;
        } else if (rc == 0) {
            *ptr = 0;
            return n - 1;
        } else {
            if (errno == EINTR) goto again;
            return -1;
        }
    }
    *ptr = 0;
    return n;
}

uint64_t get_timestamp_ms() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1000ULL + ts.tv_nsec / 1000000ULL;
}

void hexdump(const void *data, size_t len) {
    const unsigned char *p = (const unsigned char*)data;
    for (size_t i = 0; i < len; i += 16) {
        printf("%08zx  ", i);
        for (size_t j = 0; j < 16; j++) {
            if (i + j < len) printf("%02x ", p[i+j]);
            else printf("   ");
        }
        printf(" |");
        for (size_t j = 0; j < 16 && i + j < len; j++) {
            printf("%c", isprint(p[i+j]) ? p[i+j] : '.');
        }
        printf("|\n");
    }
}

// 简单HTTP请求解析
int http_parse_request(const char *buf, size_t len, http_request_t *req) {
    memset(req, 0, sizeof(*req));
    req->keep_alive = 1;
    
    const char *line_end = strstr(buf, "\r\n");
    if (!line_end) return -1;
    char first_line[2048];
    size_t first_len = line_end - buf;
    strncpy(first_line, buf, first_len);
    first_line[first_len] = '\0';
    
    if (sscanf(first_line, "%15s %1023s %15s", req->method, req->path, req->version) != 3) {
        return -1;
    }
    
    const char *p = line_end + 2;
    while (p < buf + len && strncmp(p, "\r\n", 2) != 0) {
        line_end = strstr(p, "\r\n");
        if (!line_end) break;
        char line[1024];
        size_t llen = line_end - p;
        strncpy(line, p, llen);
        line[llen] = '\0';
        
        char key[128], value[512];
        if (sscanf(line, "%127[^:]: %511[^\r\n]", key, value) == 2) {
            if (strcasecmp(key, "Host") == 0) strncpy(req->host, value, sizeof(req->host)-1);
            else if (strcasecmp(key, "Content-Type") == 0) strncpy(req->content_type, value, sizeof(req->content_type)-1);
            else if (strcasecmp(key, "Content-Length") == 0) req->content_length = atol(value);
            else if (strcasecmp(key, "Connection") == 0) {
                if (strcasecmp(value, "close") == 0) req->keep_alive = 0;
            }
        }
        p = line_end + 2;
    }
    
    if (req->content_length > 0 && strncmp(p, "\r\n", 2) == 0) {
        p += 2;
        size_t body_avail = len - (p - buf);
        req->body = (char*)p;
        req->body_len = body_avail < (size_t)req->content_length ? body_avail : req->content_length;
    }
    return 0;
}

void http_build_response(char *buf, size_t buf_len, int status,
                         const char *content_type, const char *body, int keep_alive) {
    const char *reason = "OK";
    if (status == 404) reason = "Not Found";
    else if (status == 400) reason = "Bad Request";
    else if (status == 500) reason = "Internal Server Error";
    
    long content_len = body ? strlen(body) : 0;
    int n = snprintf(buf, buf_len,
        "HTTP/1.1 %d %s\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %ld\r\n"
        "Connection: %s\r\n"
        "Server: C-Net-Demo/1.0\r\n"
        "\r\n",
        status, reason,
        content_type ? content_type : "text/plain",
        content_len,
        keep_alive ? "keep-alive" : "close");
    
    if (body && n > 0 && (size_t)n < buf_len) {
        strncpy(buf + n, body, buf_len - n - 1);
    }
}
