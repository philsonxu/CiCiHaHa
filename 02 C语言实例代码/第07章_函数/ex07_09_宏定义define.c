/*
 * 例7.9 函数与宏对比（注：宏是预处理指令，本章了解对比）
 * 知识点：宏定义在预处理阶段展开，函数在运行时调用
 */
#include <stdio.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))  /* 带参数的宏 */
#define SQUARE(x) ((x)*(x))

int max_func(int a, int b)
{
    return (a > b) ? a : b;
}

int main()
{
    int x = 3, y = 5;
    printf("宏MAX(%d,%d) = %d\n", x, y, MAX(x, y));
    printf("函数max_func(%d,%d) = %d\n", x, y, max_func(x, y));
    
    /* 注意宏的副作用 */
    int i = 3;
    printf("SQUARE(i++) = %d\n", SQUARE(i++));  /* 展开为((i++)*(i++))，结果可能非预期 */
    printf("i执行后值：%d\n", i);
    
    return 0;
}