/*
 * 例11.2 带参数的宏定义
 * 知识点：宏参数替换、注意括号使用（防止运算优先级问题）
 * 宏与函数的区别：宏无类型检查、直接展开可能使代码变长
 */
#include <stdio.h>

/* 每个参数和整体都要加括号！ */
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define SQUARE(x) ((x) * (x))
#define ABS(x) ((x) >= 0 ? (x) : -(x))

/* 不带括号的危险示范 */
#define BAD_SQUARE(x) x*x  /* 错误：SQUARE(3+2) 变成 3+2*3+2=11，非预期 */

int main()
{
    int a = 3, b = 5;
    printf("MAX(%d,%d) = %d\n", a, b, MAX(a, b));
    printf("MIN(%d,%d) = %d\n", a, b, MIN(a, b));
    printf("SQUARE(%d) = %d\n", a, SQUARE(a));
    printf("SQUARE(3+2) = %d  括号保证结果正确\n", SQUARE(3 + 2));
    printf("BAD_SQUARE(3+2) = %d  没有括号导致错误！\n", BAD_SQUARE(3 + 2));
    printf("ABS(-10) = %d\n", ABS(-10));
    
    return 0;
}