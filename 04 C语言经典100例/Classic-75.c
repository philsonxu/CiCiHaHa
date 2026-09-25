#include <stdio.h>

/*
* C 语言经典100例-75
* https://www.runoob.com/cprogramming/c-exercise-example75.html
* 题目：整数输反
* 转入一个整数，并将其反转后输出。
*/
int main()
{
    int n, reversedNumber = 0, remainder;

    printf("整数输反\n");
    printf("输入一个整数: ");
    scanf("%d", &n);

    while (n != 0)
    {
        remainder = n % 10;
        reversedNumber = reversedNumber * 10 + remainder;
        n /= 10;
    }

    printf("反转后的整数: %d", reversedNumber);

    return 0;
}