/*
 * 例12.2 位运算典型应用
 * 知识点：用位运算对特定二进制位进行置1、清0、翻转、检查
 */
#include <stdio.h>

/* 设置第n位为1（n从0开始，从最低位算起）*/
#define SET_BIT(x, n)   ((x) | (1U << (n)))

/* 将第n位清0 */
#define CLEAR_BIT(x, n) ((x) & ~(1U << (n)))

/* 翻转第n位（0变1，1变0）*/
#define TOGGLE_BIT(x, n) ((x) ^ (1U << (n)))

/* 检查第n位是否为1 */
#define CHECK_BIT(x, n) (((x) >> (n)) & 1U)

int main()
{
    unsigned int flags = 0;
    printf("初始flags = %u\n", flags);
    
    /* 设置第0位和第3位 */
    flags = SET_BIT(flags, 0);
    flags = SET_BIT(flags, 3);
    printf("设置第0位和第3位后 = %u\n", flags);
    
    /* 检查各位 */
    int i;
    printf("各位状态：");
    for (i = 7; i >= 0; i--) {
        printf("%d", CHECK_BIT(flags, i));
    }
    printf("\n");
    
    /* 清除第0位 */
    flags = CLEAR_BIT(flags, 0);
    printf("清除第0位后 = %u\n", flags);
    
    /* 翻转第1位和第3位 */
    flags = TOGGLE_BIT(flags, 1);
    flags = TOGGLE_BIT(flags, 3);
    printf("翻转第1位和第3位后 = %u\n", flags);
    
    /* 实际应用：用位表示多个开关状态 */
    printf("\n--- 权限模拟（读/写/执行） ---\n");
#define READ   (1U << 0)  /* 读权限 = 1 (001) */
#define WRITE  (1U << 1)  /* 写权限 = 2 (010) */
#define EXEC   (1U << 2)  /* 执行权限 = 4 (100) */
    
    unsigned char perm = READ | WRITE;  /* 读+写 = 3 */
    printf("perm = %u\n", perm);
    if (perm & READ)  printf("有读权限\n");
    if (perm & WRITE) printf("有写权限\n");
    if (perm & EXEC)  printf("有执行权限\n");
    else printf("无执行权限\n");
    
    return 0;
}