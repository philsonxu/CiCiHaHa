CC = gcc
CFLAGS = -Wall -O2 -Icommon -D_GNU_SOURCE
LDFLAGS = -lpthread -lm

COMMON_OBJ = common/common.o

.PHONY: all clean run

all: $(COMMON_OBJ) \
     01_tcp_basic/tcp_echo_iterative \
     01_tcp_basic/tcp_echo_fork \
     01_tcp_basic/tcp_echo_pthread \
     01_tcp_basic/tcp_benchmark_client \
     02_udp_basic/udp_echo_server \
     02_udp_basic/udp_echo_client \
     02_udp_basic/udp_broadcast_sender \
     02_udp_basic/udp_broadcast_receiver \
     03_socket_options/socket_options_demo \
     04_io_multiplexing/select_server \
     04_io_multiplexing/poll_server \
     05_epoll_server/epoll_lt_server \
     05_epoll_server/epoll_et_server \
     06_reactor_pattern/reactor_server \
     07_http_server/http_server \
     09_udp_reliable/rudp_demo \
     10_network_tools/port_scanner

# 注意：websocket需要libcrypto，ping/traceroute需要root权限运行，单独编译为可选
all_full: all \
     08_websocket/ws_server \
     10_network_tools/my_ping \
     10_network_tools/my_traceroute

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

01_tcp_basic/tcp_echo_iterative: 01_tcp_basic/tcp_echo_server_iterative.c $(COMMON_OBJ)
	$(CC) $(CFLAGS) $< $(COMMON_OBJ) -o $@ $(LDFLAGS)

01_tcp_basic/tcp_echo_fork: 01_tcp_basic/tcp_echo_server_fork.c $(COMMON_OBJ)
	$(CC) $(CFLAGS) $< $(COMMON_OBJ) -o $@ $(LDFLAGS)

01_tcp_basic/tcp_echo_pthread: 01_tcp_basic/tcp_echo_server_pthread.c $(COMMON_OBJ)
	$(CC) $(CFLAGS) $< $(COMMON_OBJ) -o $@ $(LDFLAGS)

01_tcp_basic/tcp_benchmark_client: 01_tcp_basic/tcp_benchmark_client.c $(COMMON_OBJ)
	$(CC) $(CFLAGS) $< $(COMMON_OBJ) -o $@ $(LDFLAGS)

02_udp_basic/udp_echo_server: 02_udp_basic/udp_echo_server.c $(COMMON_OBJ)
	$(CC) $(CFLAGS) $< $(COMMON_OBJ) -o $@ $(LDFLAGS)

02_udp_basic/udp_echo_client: 02_udp_basic/udp_echo_client.c $(COMMON_OBJ)
	$(CC) $(CFLAGS) $< $(COMMON_OBJ) -o $@ $(LDFLAGS)

02_udp_basic/udp_broadcast_sender: 02_udp_basic/udp_broadcast_sender.c $(COMMON_OBJ)
	$(CC) $(CFLAGS) $< $(COMMON_OBJ) -o $@ $(LDFLAGS)

02_udp_basic/udp_broadcast_receiver: 02_udp_basic/udp_broadcast_receiver.c $(COMMON_OBJ)
	$(CC) $(CFLAGS) $< $(COMMON_OBJ) -o $@ $(LDFLAGS)

03_socket_options/socket_options_demo: 03_socket_options/socket_options_demo.c $(COMMON_OBJ)
	$(CC) $(CFLAGS) $< $(COMMON_OBJ) -o $@ $(LDFLAGS)

04_io_multiplexing/select_server: 04_io_multiplexing/select_server.c $(COMMON_OBJ)
	$(CC) $(CFLAGS) $< $(COMMON_OBJ) -o $@ $(LDFLAGS)

04_io_multiplexing/poll_server: 04_io_multiplexing/poll_server.c $(COMMON_OBJ)
	$(CC) $(CFLAGS) $< $(COMMON_OBJ) -o $@ $(LDFLAGS)

05_epoll_server/epoll_lt_server: 05_epoll_server/epoll_server_lt.c $(COMMON_OBJ)
	$(CC) $(CFLAGS) $< $(COMMON_OBJ) -o $@ $(LDFLAGS)

05_epoll_server/epoll_et_server: 05_epoll_server/epoll_server_et.c $(COMMON_OBJ)
	$(CC) $(CFLAGS) $< $(COMMON_OBJ) -o $@ $(LDFLAGS)

06_reactor_pattern/reactor_server: 06_reactor_pattern/reactor_echo_server.c $(COMMON_OBJ)
	$(CC) $(CFLAGS) $< $(COMMON_OBJ) -o $@ $(LDFLAGS)

07_http_server/http_server: 07_http_server/http_static_server.c $(COMMON_OBJ)
	$(CC) $(CFLAGS) $< $(COMMON_OBJ) -o $@ $(LDFLAGS)

08_websocket/ws_server: 08_websocket/ws_echo_server.c $(COMMON_OBJ)
	$(CC) $(CFLAGS) $< $(COMMON_OBJ) -o $@ $(LDFLAGS) -lcrypto

09_udp_reliable/rudp_demo: 09_udp_reliable/rudp_echo_demo.c $(COMMON_OBJ)
	$(CC) $(CFLAGS) $< $(COMMON_OBJ) -o $@ $(LDFLAGS)

10_network_tools/my_ping: 10_network_tools/my_ping.c $(COMMON_OBJ)
	$(CC) $(CFLAGS) $< $(COMMON_OBJ) -o $@ $(LDFLAGS)

10_network_tools/my_traceroute: 10_network_tools/my_traceroute.c $(COMMON_OBJ)
	$(CC) $(CFLAGS) $< $(COMMON_OBJ) -o $@ $(LDFLAGS)

10_network_tools/port_scanner: 10_network_tools/port_scanner.c $(COMMON_OBJ)
	$(CC) $(CFLAGS) $< $(COMMON_OBJ) -o $@ $(LDFLAGS)

# 运行socket选项演示作为快速验证
run: all
	@echo "===== 运行Socket选项演示验证 ====="
	./03_socket_options/socket_options_demo
	@echo "===== 编译验证通过，所有示例已就绪 ====="

clean:
	rm -f $(COMMON_OBJ)
	rm -f 01_tcp_basic/tcp_echo_iterative 01_tcp_basic/tcp_echo_fork 01_tcp_basic/tcp_echo_pthread 01_tcp_basic/tcp_benchmark_client
	rm -f 02_udp_basic/udp_echo_server 02_udp_basic/udp_echo_client 02_udp_basic/udp_broadcast_sender 02_udp_basic/udp_broadcast_receiver
	rm -f 03_socket_options/socket_options_demo
	rm -f 04_io_multiplexing/select_server 04_io_multiplexing/poll_server
	rm -f 05_epoll_server/epoll_lt_server 05_epoll_server/epoll_et_server
	rm -f 06_reactor_pattern/reactor_server
	rm -f 07_http_server/http_server
	rm -f 08_websocket/ws_server
	rm -f 09_udp_reliable/rudp_demo
	rm -f 10_network_tools/my_ping 10_network_tools/my_traceroute 10_network_tools/port_scanner
