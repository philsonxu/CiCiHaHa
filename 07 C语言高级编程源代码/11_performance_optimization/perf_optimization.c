/**
 * ============================================================================
 * C语言性能优化技巧
 * 编译：gcc -Wall -std=c99 -O0 -o perf_optimization perf_optimization.c
 *   （建议加 -O0 防止编译器直接优化掉要对比的代码，实际发布用 -O2）
 * ============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

#define ARR_SIZE 10000000   // 1000万元素
#define REPEAT   32

static double now_sec() {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec + t.tv_nsec * 1e-9;
}

/* ==========================================================================
 * 1. 位运算代替乘除模
 * ========================================================================== */
void demo_bit_operation() {
    printf("\n===== 1. 位运算代替乘除模 =====\n");
    int x = 1234567;
    double t0, t1;
    volatile int sum;

    /* 乘法：x * 2, x * 4, x * 8 ... -> x << n */
    t0 = now_sec();
    sum = 0;
    for (int i = 0; i < REPEAT; i++) {
        for (volatile int j = 0; j < 1000000; j++) sum += x * 8;
    }
    t1 = now_sec();
    printf("乘法  x * 8     : %.3f s\n", t1 - t0);

    t0 = now_sec();
    sum = 0;
    for (int i = 0; i < REPEAT; i++) {
        for (volatile int j = 0; j < 1000000; j++) sum += x << 3;
    }
    t1 = now_sec();
    printf("位运算 x << 3   : %.3f s\n", t1 - t0);

    /* 模运算：判断奇偶：x % 2 == 0 -> (x & 1) == 0 */
    int even_count = 0;
    int nums[] = {1,2,3,4,5,6,7,8,9,10};
    for (int i = 0; i < 10; i++) {
        if ((nums[i] & 1) == 0) even_count++;
    }
    printf("用 (x&1)==0 判断偶数，数组中共 %d 个偶数\n", even_count);

    /* 交换两个数（不使用临时变量） */
    int a = 10, b = 20;
    printf("交换前：a=%d, b=%d\n", a, b);
    a ^= b; b ^= a; a ^= b;
    printf("异或交换后：a=%d, b=%d\n", a, b);
}

/* ==========================================================================
 * 2. 循环优化：循环展开、不变量外提
 * ========================================================================== */
void demo_loop_optimization() {
    printf("\n===== 2. 循环优化 =====\n");
    int *arr = (int *)malloc(sizeof(int) * ARR_SIZE);
    for (int i = 0; i < ARR_SIZE; i++) arr[i] = i + 1;

    /* 普通求和 */
    volatile long long sum1 = 0;
    double t0 = now_sec();
    for (int i = 0; i < ARR_SIZE; i++) {
        sum1 += arr[i];
    }
    double t1 = now_sec();
    printf("普通求和:          %.4f s, sum=%lld\n", t1 - t0, sum1);

    /* 循环展开：一次循环处理4个元素，减少循环次数/分支预测 */
    volatile long long sum2 = 0;
    t0 = now_sec();
    int i = 0;
    for (; i <= ARR_SIZE - 4; i += 4) {
        sum2 += arr[i] + arr[i+1] + arr[i+2] + arr[i+3];
    }
    for (; i < ARR_SIZE; i++) sum2 += arr[i];
    t1 = now_sec();
    printf("4路循环展开求和:   %.4f s, sum=%lld\n", t1 - t0, sum2);

    free(arr);
}

/* ==========================================================================
 * 3. 内存对齐：提高访问效率、避免跨缓存行
 * ========================================================================== */
void demo_memory_alignment() {
    printf("\n===== 3. 内存对齐与缓存行 =====\n");
    /* 未对齐的结构体：实际占用会比想象中大 */
    struct Bad {
        char c;     // 1字节
        int i;      // 4字节（需4字节对齐）
        short s;    // 2字节
    }; // 编译器会插入填充字节！

    struct Good {
        int i;      // 4字节
        short s;    // 2字节
        char c;     // 1字节
        // 末尾补1字节让整体对齐到4
    };

    printf("struct Bad  大小: %zu 字节（c,i,s 顺序，填充浪费）\n", sizeof(struct Bad));
    printf("struct Good 大小: %zu 字节（按大小从大到小排列）\n", sizeof(struct Good));

    /* 缓存行对齐：防止伪共享（false sharing）
     * 多线程读写同一条缓存行的不同变量会互相无效化 */
    printf("\n缓存行大小通常为 64 字节\n");
    printf("多线程场景中，独立频繁更新的变量应用 __attribute__((aligned(64))) 隔离\n");
}

/* ==========================================================================
 * 4. 查表法（空间换时间）
 * ========================================================================== */
int is_upper_branch(char c) {
    return (c >= 'A' && c <= 'Z');  // 每次都要比较
}

/* 预先构建查表数组 */
static uint8_t upper_table[256];
void build_upper_table() {
    memset(upper_table, 0, 256);
    for (int c = 'A'; c <= 'Z'; c++) upper_table[c] = 1;
}
int is_upper_table(char c) {
    return upper_table[(unsigned char)c];
}

void demo_lookup_table() {
    printf("\n===== 4. 查表法（空间换时间）=====\n");
    build_upper_table();

    const char *test_str = "Hello World! C Advanced Programming 123";
    int len = strlen(test_str);
    const int ROUNDS = 1000000;

    /* 分支版本 */
    int count1 = 0;
    double t0 = now_sec();
    for (int r = 0; r < ROUNDS; r++) {
        count1 = 0;
        for (int i = 0; i < len; i++) {
            if (is_upper_branch(test_str[i])) count1++;
        }
    }
    double t1 = now_sec();
    printf("分支法统计大写字母:   %.3f s, 大写数=%d\n", t1 - t0, count1);

    /* 查表版本 */
    int count2 = 0;
    t0 = now_sec();
    for (int r = 0; r < ROUNDS; r++) {
        count2 = 0;
        for (int i = 0; i < len; i++) {
            if (is_upper_table(test_str[i])) count2++;
        }
    }
    t1 = now_sec();
    printf("查表法统计大写字母:   %.3f s, 大写数=%d\n", t1 - t0, count2);
}

/* ==========================================================================
 * 5. 内联函数 vs 宏
 * ========================================================================== */
static inline int square_inline(int x) { return x * x; }
#define SQUARE_MACRO(x) ((x)*(x))

void demo_inline() {
    printf("\n===== 5. inline 与宏对比 =====\n");
    int a = 5;
    printf("square_inline(5) = %d\n", square_inline(a));
    printf("SQUARE_MACRO(5)  = %d\n", SQUARE_MACRO(a));
    printf("\ninline 函数优势：\n");
    printf("  - 有类型检查，更安全\n");
    printf("  - 参数只求值一次（宏可能多次求值导致BUG）\n");
    printf("  - 可以调试（宏展开后代码不可见）\n");
}

/* ==========================================================================
 * 主函数
 * ========================================================================== */
int main() {
    printf("=======================================\n");
    printf("   C语言性能优化技巧（-O0 编译对比）    \n");
    printf("=======================================\n");

    demo_bit_operation();
    demo_loop_optimization();
    demo_memory_alignment();
    demo_lookup_table();
    demo_inline();

    printf("\n性能优化原则：\n");
    printf("  1. 先 profiling，后优化（不要凭感觉优化）\n");
    printf("  2. 算法复杂度 > 常量优化（O(nlogn) 永远比 O(n^2) 快）\n");
    printf("  3. 优化热点代码（80%%的时间在20%%的代码中）\n");
    printf("  4. 利用 CPU 缓存：顺序访问 > 随机访问\n");
    printf("  5. 减少分支预测失败：使用 likely/unlikely、无分支写法\n");
    printf("  6. 避免过早优化：可读性优先\n");

    return 0;
}
