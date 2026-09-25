/*
 * 例5.1 while循环 - 求1+2+...+100的和
 * 知识点：while循环结构、循环变量初始化与更新、累加算法
 */
#include <stdio.h>

int main()
{
    int i = 1, sum = 0;
    while (i <= 100) {
        sum = sum + i;
        i++;
    }
    printf("1+2+...+100 = %d\n", sum);
    return 0;
}