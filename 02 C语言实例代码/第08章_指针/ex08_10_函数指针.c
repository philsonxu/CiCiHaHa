/*
 * 例8.10 指向函数的指针（函数指针）
 * 知识点：函数名是函数入口地址、通过函数指针调用函数
 */
#include <stdio.h>

int imax(int a, int b)
{
    return (a > b) ? a : b;
}

int imin(int a, int b)
{
    return (a < b) ? a : b;
}

int main()
{
    int (*p)(int, int);  /* 定义函数指针 */
    int a = 3, b = 5;
    
    p = imax;  /* 指向max函数 */
    printf("max(%d,%d) = %d\n", a, b, p(a, b));
    
    p = imin;  /* 指向min函数 */
    printf("min(%d,%d) = %d\n", a, b, p(a, b));
    
    return 0;
}