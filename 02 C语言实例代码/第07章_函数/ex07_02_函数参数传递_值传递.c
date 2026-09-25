/*
 * 例7.2 函数参数传递 - 值传递
 * 知识点：实参与形参、单向值传递、形参变化不影响实参
 */
#include <stdio.h>

void swap(int x, int y);  /* 声明交换函数 */

int main()
{
    int a = 3, b = 5;
    printf("交换前：a=%d, b=%d\n", a, b);
    swap(a, b);
    printf("交换后：a=%d, b=%d  （值传递：实参a,b并未真正交换）\n", a, b);
    return 0;
}

/* 错误的交换：值传递无法改变实参 */
void swap(int x, int y)
{
    int t;
    t = x;
    x = y;
    y = t;
    printf("函数内：x=%d, y=%d\n", x, y);
}