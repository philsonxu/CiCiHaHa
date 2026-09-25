/**
 * ============================================================================
 * C语言高级预处理宏技巧
 * 编译：gcc -Wall -std=c99 -o macro_advanced macro_advanced.c
 * ============================================================================
 */
#include <stdio.h>
#include <stdlib.h>

/* ==========================================================================
 * 1. 字符串化（#）与连接（##）
 * ========================================================================== */
#define STRINGIFY(x)       #x            // 把参数转成字符串
#define CONCAT(a, b)       a##b          // 连接两个标识符
#define PRINT_VAR(var)     printf(#var " = %d\n", var)

/* ==========================================================================
 * 2. 多行宏（do { } while(0) 经典技巧）
 *    作用：在 if/else 中调用时不会产生语法错误
 * ========================================================================== */
#define SWAP(a, b)  do {     \
    typeof(a) _tmp = (a);    \
    (a) = (b);               \
    (b) = _tmp;              \
} while(0)

/* ==========================================================================
 * 3. 通用最大值/最小值宏（GCC 扩展 typeof）
 * ========================================================================== */
#define MAX(a, b) ({          \
    typeof(a) _a = (a);       \
    typeof(b) _b = (b);       \
    _a > _b ? _a : _b;        \
})
#define MIN(a, b) ({          \
    typeof(a) _a = (a);       \
    typeof(b) _b = (b);       \
    _a < _b ? _a : _b;        \
})

/* ==========================================================================
 * 4. 编译时断言（C11 前的传统做法）
 * ========================================================================== */
#define STATIC_ASSERT(cond, msg) \
    typedef char CONCAT(static_assert_fail_, __LINE__)[(cond) ? 1 : -1]

STATIC_ASSERT(sizeof(int) == 4, int必须是4字节);
STATIC_ASSERT(sizeof(char) == 1, char必须是1字节);

/* ==========================================================================
 * 5. 可变参数宏（__VA_ARGS__）
 * ========================================================================== */
#define LOG_DEBUG(fmt, ...) \
    printf("[DEBUG %s:%d] " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)

#define LOG_ERROR(fmt, ...) \
    fprintf(stderr, "[ERROR %s:%d] " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)

/* ==========================================================================
 * 6. 获取数组成员个数
 * ========================================================================== */
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

/* ==========================================================================
 * 7. 结构体偏移量（等价于 stddef.h 的 offsetof）
 * ========================================================================== */
#define MY_OFFSETOF(type, member) ((size_t)&(((type *)0)->member))

/* ==========================================================================
 * 8. X-Macro 技巧：通过一个宏列表生成枚举+字符串数组（避免重复代码）
 * ========================================================================== */
/* 定义颜色列表（唯一数据来源） */
#define COLOR_LIST(X)  \
    X(RED)             \
    X(GREEN)           \
    X(BLUE)            \
    X(YELLOW)          \
    X(BLACK)           \
    X(WHITE)

/* 用 X-Macro 生成枚举 */
#define DEFINE_ENUM(c) c,
typedef enum {
    COLOR_LIST(DEFINE_ENUM)
    COLOR_COUNT
} Color;
#undef DEFINE_ENUM

/* 用 X-Macro 生成名字字符串数组 */
#define DEFINE_NAME(c) #c,
const char *color_names[] = {
    COLOR_LIST(DEFINE_NAME)
};
#undef DEFINE_NAME

/* ==========================================================================
 * 9. 分支预测提示（GCC/Clang __builtin_expect）
 * ========================================================================== */
#define LIKELY(x)   __builtin_expect(!!(x), 1)
#define UNLIKELY(x) __builtin_expect(!!(x), 0)

/* ==========================================================================
 * 主函数
 * ========================================================================== */
int main() {
    printf("=======================================\n");
    printf("   C语言高级预处理宏技巧               \n");
    printf("=======================================\n\n");

    // 1. 字符串化、连接
    printf("===== 字符串化与连接 =====\n");
    printf("STRINGIFY(hello world) = %s\n", STRINGIFY(hello world));
    int CONCAT(my_, var) = 42;  // 展开成 int my_var = 42;
    printf("CONCAT(my_, var) 展开后变量名是 my_var，值为 %d\n", my_var);
    PRINT_VAR(my_var);

    // 2. do-while(0) 多行宏
    printf("\n===== SWAP 宏 =====\n");
    int x = 10, y = 20;
    printf("交换前：x=%d, y=%d\n", x, y);
    SWAP(x, y);
    printf("交换后：x=%d, y=%d\n", x, y);

    // 3. MAX/MIN 泛型宏
    printf("\n===== 泛型 MAX/MIN =====\n");
    printf("MAX(3, 7) = %d\n", MAX(3, 7));
    printf("MAX(3.14, 2.718) = %.3f\n", MAX(3.14, 2.718));
    printf("MIN(100, 42) = %d\n", MIN(100, 42));

    // 4. 日志宏
    printf("\n===== 可变参数日志宏 =====\n");
    LOG_DEBUG("程序启动，版本 %d.%d", 1, 0);
    int ret = 0;
    if (ret != 0) {
        LOG_ERROR("操作失败，错误码=%d", ret);
    }

    // 5. 数组大小
    int numbers[] = {1, 2, 3, 4, 5, 6, 7};
    printf("\n===== 数组大小 =====\n");
    printf("numbers 数组元素个数：%zu\n", ARRAY_SIZE(numbers));

    // 6. 结构体偏移
    struct Person {
        char name[20];
        int age;
        double salary;
    };
    printf("\n===== 结构体偏移量 =====\n");
    printf("struct Person：name偏移=%zu, age偏移=%zu, salary偏移=%zu, 大小=%zu\n",
           MY_OFFSETOF(struct Person, name),
           MY_OFFSETOF(struct Person, age),
           MY_OFFSETOF(struct Person, salary),
           sizeof(struct Person));

    // 7. X-Macro
    printf("\n===== X-Macro 生成的颜色枚举 =====\n");
    for (int i = 0; i < COLOR_COUNT; i++) {
        printf("  颜色[%d] = %s\n", i, color_names[i]);
    }

    // 8. 分支预测提示
    printf("\n===== 分支预测提示 =====\n");
    int err = 0;
    if (LIKELY(err == 0)) {
        printf("正常路径（提示CPU优先预测此分支）\n");
    }

    printf("\n常用预定义宏：\n");
    printf("  __FILE__    当前文件: %s\n", __FILE__);
    printf("  __LINE__    当前行号: %d\n", __LINE__);
    printf("  __func__    当前函数: %s\n", __func__);
    printf("  __DATE__    编译日期: %s\n", __DATE__);
    printf("  __TIME__    编译时间: %s\n", __TIME__);
    printf("  __STDC__    是否标准C: %d\n", __STDC__);

    return 0;
}
