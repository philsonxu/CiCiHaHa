#include<stdio.h>
#include<stdlib.h>

int is_primer(unsigned int n);

/*
* C 语言经典100例-84
* https://www.runoob.com/cprogramming/c-exercise-example84.html
* 题目：验证哥德巴赫猜想
* 一个偶数总能表示为两个素数之和。
*/
int main()
{
    printf("验证哥德巴赫猜想：一个偶数总能表示为两个素数之和\n");
    unsigned int n, i;
    do {
        printf("请输入一个偶数:\n");
        scanf("%d", &n);
    } while (n % 2 != 0);
    for (i = 1; i < n; i++)
        if (is_primer(i) && is_primer(n - i))
            break;
    printf("偶数 %d 可以分解成 %d 和 %d 两个素数的和\n", n, i, n - i);

    return 0;
}

int is_primer(unsigned int n)
{
    int i;
    if (n < 4)return 1;
    else if (n % 2 == 0)return 0;
    else
        for (i = 3; i < sqrt(n) + 1; i++)
            if (n % i == 0)return 0;

    return 1;
}