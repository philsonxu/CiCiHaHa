#include <stdio.h>
/*
* C 语言实例 - 求一个整数的所有因数
* https://www.runoob.com/cprogramming/c-examples.html
*/

int main()
{
    int number, i;

    printf("输入一个整数: ");
    scanf("%d", &number);

    printf("%d 的因数有: ", number);
    for (i = 1; i <= number; ++i)
    {
        if (number % i == 0)
        {
            printf("%d ", i);
        }
    }

    return 0;
}


int main()
{
    int num1, num2, i;
    printf("输入两个数，使用空格分隔:");
    scanf("%d %d", &num1, &num2);
    for (i = 1; i <= (num1 > num2 ? num2 : num1); ++i)
    {
        if (num1 % i == 0 && num2 % i == 0)
        {
            printf("%d ", i);
        }
    }
    return 0;
}