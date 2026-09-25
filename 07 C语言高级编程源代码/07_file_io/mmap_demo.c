/**
 * ============================================================================
 * C语言文件IO高级操作：mmap内存映射
 * 说明：mmap 将文件映射到进程虚拟地址空间，像操作内存一样操作文件
 * 优势：减少用户态/内核态数据拷贝，操作大文件效率高
 * 编译：gcc -Wall -std=c99 -D_POSIX_C_SOURCE=200809L -o mmap_demo mmap_demo.c
 * ============================================================================
 */
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <time.h>

#define TEST_FILE "mmap_test.dat"
#define FILE_SIZE (1024 * 1024)  // 1MB

/* 传统 read/write 方式复制 */
long long test_read_write(const char *src, const char *dst) {
    int fd_in = open(src, O_RDONLY);
    int fd_out = open(dst, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    char buf[4096];
    ssize_t n;
    struct timespec t1, t2;
    clock_gettime(CLOCK_MONOTONIC, &t1);

    while ((n = read(fd_in, buf, sizeof(buf))) > 0) {
        write(fd_out, buf, n);
    }

    clock_gettime(CLOCK_MONOTONIC, &t2);
    close(fd_in); close(fd_out);
    return (t2.tv_sec - t1.tv_sec) * 1000000000LL + (t2.tv_nsec - t1.tv_nsec);
}

/* mmap 方式复制 */
long long test_mmap_copy(const char *src, const char *dst) {
    int fd_in = open(src, O_RDONLY);
    int fd_out = open(dst, O_RDWR | O_CREAT | O_TRUNC, 0644);
    struct stat st;
    fstat(fd_in, &st);
    ftruncate(fd_out, st.st_size);  // 必须先设置目标文件大小

    struct timespec t1, t2;
    clock_gettime(CLOCK_MONOTONIC, &t1);

    void *in = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd_in, 0);
    void *out = mmap(NULL, st.st_size, PROT_WRITE, MAP_SHARED, fd_out, 0);

    memcpy(out, in, st.st_size);  // 内存到内存拷贝，由内核回写

    munmap(in, st.st_size);
    munmap(out, st.st_size);
    clock_gettime(CLOCK_MONOTONIC, &t2);
    close(fd_in); close(fd_out);
    return (t2.tv_sec - t1.tv_sec) * 1000000000LL + (t2.tv_nsec - t1.tv_nsec);
}

int main() {
    printf("=======================================\n");
    printf("   mmap 内存映射文件演示               \n");
    printf("=======================================\n\n");

    // 1. 创建测试文件
    printf("1. 创建 %dMB 测试文件...\n", FILE_SIZE / 1024 / 1024);
    int fd = open(TEST_FILE, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) { perror("open"); return 1; }
    ftruncate(fd, FILE_SIZE);

    char *base = mmap(NULL, FILE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (base == MAP_FAILED) { perror("mmap"); close(fd); return 1; }

    // 像操作数组一样写文件
    for (int i = 0; i < FILE_SIZE; i++) {
        base[i] = (char)(i & 0xFF);
    }
    printf("   写入完成，首字节=0x%02X, 末字节=0x%02X\n",
           (unsigned char)base[0], (unsigned char)base[FILE_SIZE - 1]);
    msync(base, FILE_SIZE, MS_SYNC);  // 同步回磁盘
    munmap(base, FILE_SIZE);
    close(fd);

    // 2. mmap 直接修改文件（随机访问）
    printf("\n2. 使用 mmap 随机修改文件偏移 100、10000 处的值...\n");
    fd = open(TEST_FILE, O_RDWR);
    base = mmap(NULL, FILE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    printf("   修改前：base[100]=%d, base[10000]=%d\n", base[100], base[10000]);
    base[100]   = 99;
    base[10000] = 88;
    printf("   修改后：base[100]=%d, base[10000]=%d\n", base[100], base[10000]);
    munmap(base, FILE_SIZE);
    close(fd);

    // 3. 性能对比
    printf("\n3. 文件复制性能对比（1MB文件）：\n");
    long long t1 = test_read_write(TEST_FILE, "copy_rw.dat");
    long long t2 = test_mmap_copy(TEST_FILE, "copy_mmap.dat");
    printf("   read/write 方式: %lld us\n", t1 / 1000);
    printf("   mmap 方式:       %lld us\n", t2 / 1000);
    printf("   * 大文件场景下 mmap 优势更明显\n");

    unlink(TEST_FILE);
    unlink("copy_rw.dat");
    unlink("copy_mmap.dat");

    printf("\nmmap 应用场景：\n");
    printf("  1. 大文件读写（避免多次 read/write 拷贝）\n");
    printf("  2. 多进程共享内存（MAP_SHARED）\n");
    printf("  3. 动态库加载（操作系统就是用 mmap 加载 .so 的）\n");
    printf("  4. 高性能文件IO（数据库、Redis 持久化）\n");

    return 0;
}
