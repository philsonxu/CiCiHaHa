/*
 * 例1.3 求三个数的最大值
 * 知识点：变量定义、scanf输入、if条件判断、printf输出格式化
 */
#include <stdio.h>

int main()
{
    int a, b, c, max;
    printf("please input a,b,c:\n");
    scanf("%d,%d,%d", &a, &b, &c);
    max = a;
    if (max < b)
        max = b;
    if (max < c)
        max = c;
    printf("The largest number is %d\n", max);
    return 0;
}