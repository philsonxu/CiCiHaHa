/*
 * 例5.4 嵌套循环 - 输出九九乘法表
 * 知识点：双重循环、外层控制行、内层控制列、格式对齐
 */
#include <stdio.h>

int main()
{
    int i, j;
    for (i = 1; i <= 9; i++) {
        for (j = 1; j <= i; j++) {
            printf("%d*%d=%-2d ", j, i, i * j);
        }
        printf("\n");
    }
    return 0;
}