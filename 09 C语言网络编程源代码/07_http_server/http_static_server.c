/*
 * 简易HTTP静态文件服务器（Reactor模式+ET+sendfile零拷贝）
 * 功能：
 *   1. GET请求处理
 *   2. 静态文件服务，自动识别Content-Type
 *   3. 目录列表（自动生成index）
 *   4. sendfile零拷贝发送文件（避免内核态<->用户态内存拷贝）
 *   5. 简单错误页（404/400/500）
 * 编译：gcc -o http_server http_static_server.c ../common/common.c -I../common
 * 测试：启动后浏览器访问 http://127.0.0.1:8897/
 *      可放一个index.html在运行目录下测试
 */
#include "common.h"
#include <dirent.h>

#define MAX_EVENTS 1024
#define READ_BUF 8192
#define DOC_ROOT "./www"

typedef struct http_conn_s {
    int fd;
    char rbuf[READ_BUF];
    size_t rlen;
    http_request_t req;
    char wbuf[READ_BUF];
    size_t wlen;
    size_t woff;
    int filefd;         // 正在发送的文件fd
    off_t file_offset;
    off_t file_size;
    bool headers_sent;  // 响应头是否已发送
    bool keep_alive;
} http_conn_t;

const char* get_mime_type(const char *path) {
    const char *ext = strrchr(path, '.');
    if (!ext) return "text/plain";
    if (strcasecmp(ext, ".html") == 0 || strcasecmp(ext, ".htm") == 0) return "text/html; charset=utf-8";
    if (strcasecmp(ext, ".css") == 0) return "text/css";
    if (strcasecmp(ext, ".js") == 0) return "application/javascript";
    if (strcasecmp(ext, ".json") == 0) return "application/json";
    if (strcasecmp(ext, ".jpg") == 0 || strcasecmp(ext, ".jpeg") == 0) return "image/jpeg";
    if (strcasecmp(ext, ".png") == 0) return "image/png";
    if (strcasecmp(ext, ".gif") == 0) return "image/gif";
    if (strcasecmp(ext, ".ico") == 0) return "image/x-icon";
    if (strcasecmp(ext, ".txt") == 0) return "text/plain; charset=utf-8";
    return "application/octet-stream";
}

void send_error(http_conn_t *c, int status, const char *msg);
void send_directory_listing(http_conn_t *c, const char *fs_path, const char *url_path);
void send_file(http_conn_t *c, const char *fs_path);

// 处理完整HTTP请求
void process_request(http_conn_t *c) {
    if (http_parse_request(c->rbuf, c->rlen, &c->req) < 0) {
        send_error(c, 400, "Bad Request");
        return;
    }
    
    c->keep_alive = c->req.keep_alive;
    
    // 路径安全：禁止..父目录访问
    if (strstr(c->req.path, "..") != NULL) {
        send_error(c, 403, "Forbidden");
        return;
    }
    
    // 构造文件系统路径
    char fs_path[1024];
    if (strcmp(c->req.path, "/") == 0) {
        snprintf(fs_path, sizeof(fs_path), "%s", DOC_ROOT);
    } else {
        snprintf(fs_path, sizeof(fs_path), "%s%s", DOC_ROOT, c->req.path);
    }
    
    struct stat st;
    if (stat(fs_path, &st) < 0) {
        send_error(c, 404, "Not Found");
        return;
    }
    
    if (S_ISDIR(st.st_mode)) {
        // 目录：列出文件
        send_directory_listing(c, fs_path, c->req.path);
    } else if (S_ISREG(st.st_mode)) {
        // 文件：发送
        send_file(c, fs_path);
    } else {
        send_error(c, 403, "Forbidden");
    }
}

void send_error(http_conn_t *c, int status, const char *msg) {
    char body[512];
    snprintf(body, sizeof(body),
             "<html><head><title>%d %s</title></head>"
             "<body><h1>%d %s</h1><hr><i>C HTTP Server Demo</i></body></html>",
             status, msg, status, msg);
    http_build_response(c->wbuf, sizeof(c->wbuf), status, "text/html; charset=utf-8", body, 0);
    c->wlen = strlen(c->wbuf);
    c->woff = 0;
    c->filefd = -1;
}

void send_directory_listing(http_conn_t *c, const char *fs_path, const char *url_path) {
    char body[4096];
    int pos = 0;
    pos += snprintf(body + pos, sizeof(body) - pos,
                    "<html><head><meta charset='utf-8'><title>目录：%s</title></head>"
                    "<body><h1>目录列表：%s</h1><ul>", url_path, url_path);
    
    DIR *dp = opendir(fs_path);
    if (dp) {
        // 先查找index.html
        struct dirent *ep;
        bool has_index = false;
        while ((ep = readdir(dp)) != NULL) {
            if (strcmp(ep->d_name, "index.html") == 0) {
                has_index = true;
                break;
            }
        }
        closedir(dp);
        if (has_index) {
            char index_path[1024];
            snprintf(index_path, sizeof(index_path), "%s/index.html", fs_path);
            send_file(c, index_path);
            return;
        }
        
        dp = opendir(fs_path);
        while ((ep = readdir(dp)) != NULL) {
            if (ep->d_name[0] == '.') continue;
            pos += snprintf(body + pos, sizeof(body) - pos,
                            "<li><a href='%s%s'>%s</a></li>", url_path, ep->d_name, ep->d_name);
        }
        closedir(dp);
    }
    pos += snprintf(body + pos, sizeof(body) - pos, "</ul><hr><i>C HTTP Demo Server</i></body></html>");
    
    http_build_response(c->wbuf, sizeof(c->wbuf), 200, "text/html; charset=utf-8", body, c->keep_alive);
    c->wlen = strlen(c->wbuf);
    c->woff = 0;
    c->filefd = -1;
}

void send_file(http_conn_t *c, const char *fs_path) {
    const char *mime = get_mime_type(fs_path);
    
    struct stat st;
    if (stat(fs_path, &st) < 0) {
        send_error(c, 404, "Not Found");
        return;
    }
    
    c->filefd = open(fs_path, O_RDONLY);
    if (c->filefd < 0) {
        send_error(c, 500, "Internal Server Error");
        return;
    }
    c->file_offset = 0;
    c->file_size = st.st_size;
    
    // 构造响应头（分chunk发送响应头+文件体）
    char header[512];
    snprintf(header, sizeof(header),
             "HTTP/1.1 200 OK\r\n"
             "Content-Type: %s\r\n"
             "Content-Length: %ld\r\n"
             "Connection: %s\r\n"
             "Server: C-Demo/1.0\r\n"
             "\r\n",
             mime, st.st_size, c->keep_alive ? "keep-alive" : "close");
    memcpy(c->wbuf, header, strlen(header));
    c->wlen = strlen(header);
    c->woff = 0;
    c->headers_sent = false;
}

// 事件处理（简化：单进程Reactor）
int epfd;
http_conn_t* accept_conn(int listenfd);
void handle_read(http_conn_t *c);
void handle_write(http_conn_t *c);

int main(int argc, char *argv[]) {
    int port = argc > 1 ? atoi(argv[1]) : 8897;
    signal(SIGPIPE, SIG_IGN);
    
    // 创建doc root
    mkdir(DOC_ROOT, 0755);
    // 写一个测试首页
    FILE *f = fopen(DOC_ROOT"/index.html", "w");
    if (f) {
        fprintf(f, "<!DOCTYPE html><html><head><meta charset='utf-8'><title>C HTTP Server</title></head>"
                   "<body><h1>Hello from C HTTP Server!</h1>"
                   "<p>这是一个用纯C编写的简易静态文件服务器，使用epoll ET+sendfile零拷贝。</p></body></html>");
        fclose(f);
    }
    
    int listenfd = create_tcp_server("0.0.0.0", port, 1024);
    set_nonblocking(listenfd);
    epfd = epoll_create1(0);
    
    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLET;
    ev.data.ptr = (void*)(long)listenfd; // 特殊标记：监听fd
    epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &ev);
    
    log_info("HTTP静态服务器启动，端口：%d, 文档根目录：%s", port, DOC_ROOT);
    log_info("浏览器访问 http://127.0.0.1:%d/", port);
    
    struct epoll_event events[MAX_EVENTS];
    while (1) {
        int nready = epoll_wait(epfd, events, MAX_EVENTS, -1);
        for (int i = 0; i < nready; i++) {
            void *ptr = events[i].data.ptr;
            int fd = (int)(long)ptr;
            if (fd == listenfd) {
                while (1) {
                    http_conn_t *c = accept_conn(listenfd);
                    if (!c) break;
                }
                continue;
            }
            
            http_conn_t *c = (http_conn_t*)ptr;
            if (events[i].events & (EPOLLERR|EPOLLHUP|EPOLLRDHUP)) {
                if (c->filefd >= 0) close(c->filefd);
                close(c->fd);
                epoll_ctl(epfd, EPOLL_CTL_DEL, c->fd, NULL);
                free(c);
                continue;
            }
            if (events[i].events & EPOLLIN) handle_read(c);
            if (events[i].events & EPOLLOUT) handle_write(c);
        }
    }
    return 0;
}

http_conn_t* accept_conn(int listenfd) {
    struct sockaddr_in cliaddr;
    socklen_t clilen = sizeof(cliaddr);
    int connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &clilen);
    if (connfd < 0) return NULL;
    set_nonblocking(connfd);
    set_tcp_nodelay(connfd);
    
    http_conn_t *c = calloc(1, sizeof(http_conn_t));
    c->fd = connfd;
    c->filefd = -1;
    
    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLRDHUP | EPOLLET;
    ev.data.ptr = c;
    epoll_ctl(epfd, EPOLL_CTL_ADD, connfd, &ev);
    return c;
}

void handle_read(http_conn_t *c) {
    while (1) {
        ssize_t n = read(c->fd, c->rbuf + c->rlen, sizeof(c->rbuf) - c->rlen);
        if (n > 0) {
            c->rlen += n;
            // 检查是否读到完整请求头（\r\n\r\n）
            if (memmem(c->rbuf, c->rlen, "\r\n\r\n", 4)) {
                process_request(c);
                c->rlen = 0;
                // 注册写事件
                struct epoll_event ev;
                ev.events = EPOLLIN | EPOLLOUT | EPOLLRDHUP | EPOLLET;
                ev.data.ptr = c;
                epoll_ctl(epfd, EPOLL_CTL_MOD, c->fd, &ev);
                return;
            }
        } else if (n == 0) {
            // 对方关闭
            break;
        } else {
            if (errno == EAGAIN) break;
            break;
        }
    }
}

void handle_write(http_conn_t *c) {
    // 1. 先写响应头（wbuf）
    while (c->woff < c->wlen) {
        ssize_t n = write(c->fd, c->wbuf + c->woff, c->wlen - c->woff);
        if (n > 0) c->woff += n;
        else {
            if (errno == EAGAIN) return;
            goto close_conn;
        }
    }
    
    // 2. 写文件体（用sendfile零拷贝）
    if (c->filefd >= 0 && c->file_offset < c->file_size) {
        while (c->file_offset < c->file_size) {
            ssize_t n = sendfile(c->fd, c->filefd, &c->file_offset, c->file_size - c->file_offset);
            if (n == 0) break;
            if (n < 0) {
                if (errno == EAGAIN) return;
                goto close_conn;
            }
        }
        close(c->filefd);
        c->filefd = -1;
    }
    
    // 全部发送完毕
    if (c->keep_alive) {
        // 长连接：重置状态，继续等下一个请求
        c->rlen = 0;
        c->wlen = 0;
        c->woff = 0;
        struct epoll_event ev;
        ev.events = EPOLLIN | EPOLLRDHUP | EPOLLET;
        ev.data.ptr = c;
        epoll_ctl(epfd, EPOLL_CTL_MOD, c->fd, &ev);
    } else {
        goto close_conn;
    }
    return;
    
close_conn:
    if (c->filefd >= 0) close(c->filefd);
    close(c->fd);
    epoll_ctl(epfd, EPOLL_CTL_DEL, c->fd, NULL);
    free(c);
}
