#include <stdio.h>

/*
* C 语言实例 - 阶乘
* https://www.runoob.com/cprogramming/c-examples.html
*/
int main()
{
    int n, i;
    unsigned long long factorial = 1;

    printf("输入一个整数: ");
    scanf("%d", &n);

    // 如果输入是负数，显示错误
    if (n < 0)
        printf("Error! 负数没有阶乘jiechen");

    else
    {
        for (i = 1; i <= n; ++i)
        {
            factorial *= i;              // factorial = factorial*i;
        }
        printf("%d! = %llu", n, factorial);
    }

    return 0;
}

long int multiplyNumbers(int n);

int main()
{
    int n;
    printf("输入一个整数: ");
    scanf("%d", &n);
    printf("%d! = %ld", n, multiplyNumbers(n));
    return 0;
}

long int multiplyNumbers(int n)
{
    if (n > 1)
        return n * multiplyNumbers(n - 1);
    else
        return 1;
}