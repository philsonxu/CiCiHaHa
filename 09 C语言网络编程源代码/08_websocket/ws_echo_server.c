/*
 * 简易WebSocket Echo服务器
 * 实现RFC6455核心：HTTP握手(SHA1+Base64)、帧解析、帧编码、ping/pong
 * 测试：用浏览器打开下面的测试HTML页面即可连接
 * 编译：gcc -o ws_echo_server ws_echo_server.c ../common/common.c -I../common -lcrypto
 * 注意：需要openssl库做SHA1哈希（Ubuntu: apt install libssl-dev）
 *       如果不想依赖openssl，可手动实现简单的SHA1，这里用系统库保证正确
 */
#include "common.h"
#include <openssl/sha.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>

#define MAX_EVENTS 1024
#define BUF_SIZE 8192

// WebSocket操作码
#define WS_OP_TEXT   0x1
#define WS_OP_BINARY 0x2
#define WS_OP_CLOSE  0x8
#define WS_OP_PING   0x9
#define WS_OP_PONG   0xA

typedef enum {
    WS_HANDSHAKE,  // 握手阶段
    WS_MESSAGE     // 数据帧阶段
} ws_state_t;

typedef struct ws_conn_s {
    int fd;
    ws_state_t state;
    char rbuf[BUF_SIZE];
    size_t rlen;
    char wbuf[BUF_SIZE];
    size_t wlen;
} ws_conn_t;

// 全局epfd简化
int g_epfd;

// Base64编码
char* base64_encode(const unsigned char *input, int length) {
    BIO *bmem = BIO_new(BIO_s_mem());
    BIO *b64 = BIO_new(BIO_f_base64());
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
    b64 = BIO_push(b64, bmem);
    BIO_write(b64, input, length);
    BIO_flush(b64);
    BUF_MEM *bptr;
    BIO_get_mem_ptr(b64, &bptr);
    char *buff = malloc(bptr->length + 1);
    memcpy(buff, bptr->data, bptr->length);
    buff[bptr->length] = 0;
    BIO_free_all(b64);
    return buff;
}

// 构造握手响应
void ws_handshake(ws_conn_t *c) {
    // 找到Sec-WebSocket-Key
    char *key_start = strstr(c->rbuf, "Sec-WebSocket-Key:");
    if (!key_start) return;
    key_start += strlen("Sec-WebSocket-Key:");
    while (*key_start == ' ') key_start++;
    char key[64];
    int i = 0;
    while (*key_start != '\r' && *key_start != '\0' && i < 63) key[i++] = *key_start++;
    key[i] = 0;
    
    // RFC6455 magic string
    const char *magic = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
    char combine[256];
    snprintf(combine, sizeof(combine), "%s%s", key, magic);
    
    unsigned char sha[SHA_DIGEST_LENGTH];
    SHA1((unsigned char*)combine, strlen(combine), sha);
    char *accept = base64_encode(sha, SHA_DIGEST_LENGTH);
    
    const char *resp_fmt =
        "HTTP/1.1 101 Switching Protocols\r\n"
        "Upgrade: websocket\r\n"
        "Connection: Upgrade\r\n"
        "Sec-WebSocket-Accept: %s\r\n"
        "\r\n";
    c->wlen = snprintf(c->wbuf, sizeof(c->wbuf), resp_fmt, accept);
    free(accept);
    
    c->state = WS_MESSAGE;
    c->rlen = 0; // 清空读缓冲
    log_info("WebSocket握手完成");
}

// 编码一帧数据
int ws_encode_frame(char *out, size_t out_len, uint8_t op, const char *data, size_t dlen) {
    size_t pos = 0;
    // FIN + opcode
    out[pos++] = 0x80 | (op & 0x0F);
    
    // mask位=0(服务端发送不mask) + payload len
    if (dlen < 126) {
        out[pos++] = dlen & 0x7F;
    } else if (dlen < 65536) {
        out[pos++] = 126;
        out[pos++] = (dlen >> 8) & 0xFF;
        out[pos++] = dlen & 0xFF;
    } else {
        out[pos++] = 127;
        for (int i = 7; i >= 0; i--) out[pos++] = (dlen >> (i*8)) & 0xFF;
    }
    
    if (dlen > 0 && pos + dlen < out_len) {
        memcpy(out + pos, data, dlen);
        pos += dlen;
    }
    return pos;
}

// 解析并处理帧
void ws_process_frame(ws_conn_t *c) {
    while (c->rlen >= 2) {
        uint8_t *p = (uint8_t*)c->rbuf;
        uint8_t fin = (p[0] >> 7) & 1;
        uint8_t opcode = p[0] & 0x0F;
        uint8_t mask = (p[1] >> 7) & 1;
        uint64_t plen = p[1] & 0x7F;
        size_t header_len = 2;
        
        if (plen == 126) {
            if (c->rlen < 4) return;
            plen = (p[2] << 8) | p[3];
            header_len = 4;
        } else if (plen == 127) {
            if (c->rlen < 10) return;
            plen = 0;
            for (int i = 0; i < 8; i++) plen = (plen << 8) | p[2+i];
            header_len = 10;
        }
        
        uint8_t mask_key[4] = {0};
        if (mask) {
            if (c->rlen < header_len + 4) return;
            memcpy(mask_key, p + header_len, 4);
            header_len += 4;
        }
        
        if (c->rlen < header_len + plen) return; // 帧不完整
        
        char *payload = c->rbuf + header_len;
        // Unmask
        for (uint64_t i = 0; i < plen; i++) {
            payload[i] ^= mask_key[i % 4];
        }
        
        // 处理opcode
        char resp[BUF_SIZE];
        int resp_len = 0;
        if (opcode == WS_OP_TEXT || opcode == WS_OP_BINARY) {
            log_info("WS收到消息(%llu字节): %.*s", (unsigned long long)plen, (int)plen, payload);
            // Echo回显
            resp_len = ws_encode_frame(resp, sizeof(resp), opcode, payload, plen);
        } else if (opcode == WS_OP_PING) {
            resp_len = ws_encode_frame(resp, sizeof(resp), WS_OP_PONG, payload, plen);
        } else if (opcode == WS_OP_CLOSE) {
            resp_len = ws_encode_frame(resp, sizeof(resp), WS_OP_CLOSE, payload, plen > 2 ? 2 : plen);
        } else if (opcode == WS_OP_PONG) {
            // 忽略
        }
        
        if (resp_len > 0 && c->wlen + resp_len < sizeof(c->wbuf)) {
            memcpy(c->wbuf + c->wlen, resp, resp_len);
            c->wlen += resp_len;
        }
        
        // 移除已处理帧
        size_t consumed = header_len + plen;
        c->rlen -= consumed;
        memmove(c->rbuf, c->rbuf + consumed, c->rlen);
        
        if (opcode == WS_OP_CLOSE) break;
    }
}

ws_conn_t* ws_accept(int listenfd);
void ws_handle_read(ws_conn_t *c);
void ws_handle_write(ws_conn_t *c);

int main(int argc, char *argv[]) {
    int port = argc > 1 ? atoi(argv[1]) : 8898;
    signal(SIGPIPE, SIG_IGN);
    
    int listenfd = create_tcp_server("0.0.0.0", port, 1024);
    set_nonblocking(listenfd);
    g_epfd = epoll_create1(0);
    
    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLET;
    ev.data.ptr = (void*)(long)listenfd;
    epoll_ctl(g_epfd, EPOLL_CTL_ADD, listenfd, &ev);
    
    log_info("WebSocket Echo服务器启动，端口：%d", port);
    log_info("可用wscat或浏览器JS测试：new WebSocket('ws://127.0.0.1:%d/')", port);
    
    struct epoll_event events[MAX_EVENTS];
    while (1) {
        int nready = epoll_wait(g_epfd, events, MAX_EVENTS, -1);
        for (int i = 0; i < nready; i++) {
            void *ptr = events[i].data.ptr;
            int fd = (int)(long)ptr;
            if (fd == listenfd) {
                while (ws_accept(listenfd));
                continue;
            }
            ws_conn_t *c = (ws_conn_t*)ptr;
            if (events[i].events & (EPOLLERR|EPOLLHUP|EPOLLRDHUP)) {
                close(c->fd);
                epoll_ctl(g_epfd, EPOLL_CTL_DEL, c->fd, NULL);
                free(c);
                continue;
            }
            if (events[i].events & EPOLLIN) ws_handle_read(c);
            if (events[i].events & EPOLLOUT) ws_handle_write(c);
        }
    }
    return 0;
}

ws_conn_t* ws_accept(int listenfd) {
    struct sockaddr_in cliaddr;
    socklen_t clilen = sizeof(cliaddr);
    int connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &clilen);
    if (connfd < 0) return NULL;
    set_nonblocking(connfd);
    ws_conn_t *c = calloc(1, sizeof(ws_conn_t));
    c->fd = connfd;
    c->state = WS_HANDSHAKE;
    
    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLRDHUP | EPOLLET;
    ev.data.ptr = c;
    epoll_ctl(g_epfd, EPOLL_CTL_ADD, connfd, &ev);
    return c;
}

void ws_handle_read(ws_conn_t *c) {
    while (1) {
        ssize_t n = read(c->fd, c->rbuf + c->rlen, sizeof(c->rbuf) - c->rlen);
        if (n > 0) {
            c->rlen += n;
            if (c->state == WS_HANDSHAKE) {
                if (memmem(c->rbuf, c->rlen, "\r\n\r\n", 4)) {
                    ws_handshake(c);
                    struct epoll_event ev;
                    ev.events = EPOLLIN | EPOLLOUT | EPOLLRDHUP | EPOLLET;
                    ev.data.ptr = c;
                    epoll_ctl(g_epfd, EPOLL_CTL_MOD, c->fd, &ev);
                    return;
                }
            } else {
                ws_process_frame(c);
                struct epoll_event ev;
                ev.events = EPOLLIN | EPOLLOUT | EPOLLRDHUP | EPOLLET;
                ev.data.ptr = c;
                epoll_ctl(g_epfd, EPOLL_CTL_MOD, c->fd, &ev);
                return;
            }
        } else if (n <= 0) {
            if (errno == EAGAIN) break;
            break;
        }
    }
}

void ws_handle_write(ws_conn_t *c) {
    if (c->wlen == 0) {
        struct epoll_event ev;
        ev.events = EPOLLIN | EPOLLRDHUP | EPOLLET;
        ev.data.ptr = c;
        epoll_ctl(g_epfd, EPOLL_CTL_MOD, c->fd, &ev);
        return;
    }
    size_t off = 0;
    while (off < c->wlen) {
        ssize_t n = write(c->fd, c->wbuf + off, c->wlen - off);
        if (n > 0) off += n;
        else {
            if (errno == EAGAIN) {
                memmove(c->wbuf, c->wbuf + off, c->wlen - off);
                c->wlen -= off;
                return;
            }
            break;
        }
    }
    c->wlen = 0;
    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLRDHUP | EPOLLET;
    ev.data.ptr = c;
    epoll_ctl(g_epfd, EPOLL_CTL_MOD, c->fd, &ev);
}
