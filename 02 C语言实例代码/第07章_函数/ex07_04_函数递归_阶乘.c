/*
 * 例7.4 函数递归调用 - 求n的阶乘 n!
 * 知识点：递归思想、递归边界条件、递归调用链
 * n! = 1 (n=0或1)
 * n! = n*(n-1)! (n>1)
 */
#include <stdio.h>

long factorial(int n)
{
    if (n < 0) {
        printf("输入错误！\n");
        return -1;
    }
    if (n == 0 || n == 1)
        return 1;                 /* 递归终止条件 */
    else
        return n * factorial(n - 1);  /* 递归调用 */
}

int main()
{
    int n;
    printf("请输入一个正整数：");
    scanf("%d", &n);
    printf("%d! = %ld\n", n, factorial(n));
    return 0;
}