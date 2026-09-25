# C语言网络编程源代码库

一套完整的C语言网络编程学习代码库，从基础Socket到高并发服务器、应用层协议实现，涵盖Linux网络编程核心知识点。

## 目录结构

```
├── common/            # 公共工具库（Socket封装、IO函数、HTTP解析、日志）
│   ├── common.h
│   └── common.c
├── 01_tcp_basic/      # TCP基础编程
│   ├── tcp_echo_server_iterative.c   # 迭代式TCP服务器
│   ├── tcp_echo_server_fork.c        # 多进程并发服务器
│   ├── tcp_echo_server_pthread.c     # 多线程并发服务器
│   └── tcp_benchmark_client.c        # TCP性能压测客户端
├── 02_udp_basic/      # UDP基础编程
│   ├── udp_echo_server.c             # UDP回显服务器
│   ├── udp_echo_client.c             # UDP客户端
│   ├── udp_broadcast_sender.c        # UDP广播发送
│   └── udp_broadcast_receiver.c      # UDP广播接收
├── 03_socket_options/ # Socket选项详解
│   └── socket_options_demo.c         # 常用Socket选项全演示
├── 04_io_multiplexing/# IO多路复用基础
│   ├── select_server.c               # Select模型服务器
│   └── poll_server.c                 # Poll模型服务器
├── 05_epoll_server/   # Epoll高性能服务器
│   ├── epoll_server_lt.c             # Epoll LT水平触发模式
│   └── epoll_server_et.c             # Epoll ET边缘触发模式（Nginx同款）
├── 06_reactor_pattern/# Reactor事件驱动模式
│   └── reactor_echo_server.c         # 单Reactor单线程实现
├── 07_http_server/    # HTTP静态文件服务器
│   └── http_static_server.c          # epoll ET+sendfile零拷贝
├── 08_websocket/      # WebSocket协议实现
│   ├── ws_echo_server.c              # RFC6455握手+帧协议
│   └── ws_test_client.html           # 浏览器测试页面
├── 09_udp_reliable/   # 可靠UDP实现
│   └── rudp_echo_demo.c              # 序号+ACK+超时重传停等协议
└── 10_network_tools/  # 网络诊断工具
    ├── my_ping.c                     # ICMP Ping实现
    ├── my_traceroute.c               # Traceroute路由追踪
    └── port_scanner.c                # TCP非阻塞端口扫描器
```

## 编译说明

### 环境要求
- Linux 操作系统（epoll为Linux特有系统调用）
- GCC 编译器
- 部分程序需要root权限（原始套接字：ping/traceroute）
- WebSocket示例需要OpenSSL开发库：`sudo apt install libssl-dev`

### 一键编译
```bash
make          # 编译所有不依赖openssl的程序
make all_full # 编译包含WebSocket的所有程序（需要libssl-dev）
make run      # 编译并运行Socket选项演示验证
make clean    # 清理所有编译产物
```

### 单独编译某一程序
```bash
gcc -Wall -O2 -Icommon 程序路径.c common/common.o -o 输出名 -lpthread -lm
```

## 核心知识点

### 一、Socket基础
- TCP三次握手/四次挥手原理
- 迭代/多进程/多线程三种服务器模型对比
- 僵尸进程处理（SIGCHLD+waitpid）
- 线程资源回收（pthread_detach）

### 二、UDP编程
- UDP无连接特性：recvfrom/sendto无需connect
- 广播机制SO_BROADCAST（局域网设备发现原理）
- UDP与TCP的核心区别对比

### 三、Socket选项
| 选项 | 作用 |
|------|------|
| SO_REUSEADDR | 重用TIME_WAIT端口，服务器重启必备 |
| SO_REUSEPORT | 多进程端口复用，负载均衡 |
| SO_SNDBUF/SO_RCVBUF | 发送/接收缓冲区大小 |
| SO_RCVTIMEO/SO_SNDTIMEO | 收发超时 |
| TCP_NODELAY | 禁用Nagle算法，小包立即发送 |
| SO_KEEPALIVE | TCP保活探测 |
| SO_LINGER | close行为控制（优雅关闭/强制RST） |

### 四、IO多路复用演进
| 模型 | 最大并发 | 时间复杂度 | 特点 |
|------|----------|------------|------|
| select | FD_SETSIZE(1024) | O(n) | 跨平台，fd集合每次重置 |
| poll | 无硬限制 | O(n) | pollfd数组动态扩容，events/revents分离 |
| epoll | 数十万级 | O(1) | 事件驱动、回调通知、共享内存，Linux高性能首选 |

### 五、Epoll两种模式
- **LT水平触发**：缓冲区有数据就一直通知，编程简单不易出错
- **ET边缘触发**：状态变化只通知一次，必须非阻塞IO+循环读到EAGAIN，性能最高（Nginx/Redis采用）

### 六、Reactor模式
高性能网络框架的核心设计模式：
- 事件循环（Event Loop）统一监听IO事件
- 事件处理器（EventHandler）分离accept/read/write逻辑
- 非阻塞IO + 事件回调，单线程即可处理数万并发
- libevent、libuv、muduo、Netty、Node.js、Redis均采用此架构

### 七、高性能优化技术
- **零拷贝sendfile**：文件传输避免内核态<->用户态拷贝
- **writev/readv**：分散读/聚集写，减少系统调用次数
- **内存池**：减少频繁malloc/free开销
- **TCP_NODELAY**：禁用Nagle算法，降低延迟

### 八、应用层协议实现
- **HTTP协议**：请求/响应格式、Keep-Alive长连接、Content-Type
- **WebSocket**：HTTP Upgrade握手(SHA1+Base64)、二进制帧、Mask掩码、Ping/Pong心跳
- **可靠UDP**：在UDP上实现TCP核心机制——序号、确认、超时重传（KCP/QUIC基础）

## 测试方法

### 1. TCP Echo服务器测试
```bash
# 启动服务器
./01_tcp_basic/tcp_echo_pthread 8890
# 另一个终端用nc测试
nc 127.0.0.1 8890
# 压测性能
./01_tcp_basic/tcp_benchmark_client 127.0.0.1 8890 50 10000
```

### 2. Epoll ET高性能服务器压测
```bash
./05_epoll_server/epoll_et_server 8895
./01_tcp_basic/tcp_benchmark_client 127.0.0.1 8895 100 10000
# 预期QPS可达数万
```

### 3. HTTP服务器测试
```bash
./07_http_server/http_server 8897
# 浏览器访问 http://127.0.0.1:8897/
# 自动生成目录列表，支持静态文件下载
```

### 4. WebSocket测试
```bash
make all_full  # 需要先安装libssl-dev
./08_websocket/ws_server 8898
# 浏览器打开08_websocket/ws_test_client.html即可连接测试
```

### 5. 可靠UDP测试
```bash
# 终端1：启动服务器
./09_udp_reliable/rudp_demo server
# 终端2：启动客户端
./09_udp_reliable/rudp_demo client
# 观察超时重传、ACK确认过程
```

### 6. 端口扫描
```bash
# 扫描本机1-1024端口
./10_network_tools/port_scanner 127.0.0.1 1 1024
```

### 7. Ping/Traceroute（需要root）
```bash
sudo ./10_network_tools/my_ping www.baidu.com
sudo ./10_network_tools/my_traceroute www.baidu.com
```

## 学习路线建议

1. **入门**：从01_tcp_basic开始，理解TCP连接生命周期
2. **进阶**：03_socket_options掌握所有常用Socket选项
3. **提升**：04-05章节，理解IO多路复用演进，重点掌握epoll ET模式
4. **架构**：06章Reactor模式，理解高性能服务器架构设计
5. **实战**：07-08章实现HTTP/WebSocket应用层协议
6. **工具**：10章理解ICMP等底层协议，实现网络诊断工具

## 代码特点
- ✅ 每个程序独立可编译运行，无外部依赖（除libcrypto用于WebSocket的SHA1）
- ✅ 详细中文注释，关键算法逐步骤解释原理
- ✅ 全部基于POSIX标准API，可用于生产环境二次开发
- ✅ 从基础到高级循序渐进，覆盖Linux网络编程面试高频知识点
