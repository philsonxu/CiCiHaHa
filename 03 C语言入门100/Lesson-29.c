#include <stdio.h>

/*
* C 语言实例 - 计算一个数的 n 次方
* https://www.runoob.com/cprogramming/c-examples.html
*/
int main()
{
    int base, exponent;

    long long result = 1;

    printf("基数: ");
    scanf("%d", &base);

    printf("指数: ");
    scanf("%d", &exponent);

    while (exponent != 0)
    {
        result *= base;
        --exponent;
    }
    printf("结果：%lld", result);

    return 0;
}

/*======================================================================*/

int main()
{
    double base, exponent, result;

    printf("基数: ");
    scanf("%lf", &base);

    printf("指数: ");
    scanf("%lf", &exponent);

    // 计算结果
    result = pow(base, exponent);

    printf("%.1lf^%.1lf = %.2lf", base, exponent, result);

    return 0;
}

/*======================================================================*/

int power(int n1, int n2);

int main()
{
    int base, powerRaised, result;

    printf("基数: ");
    scanf("%d", &base);

    printf("指数(正整数): ");
    scanf("%d", &powerRaised);

    result = power(base, powerRaised);

    printf("%d^%d = %d", base, powerRaised, result);
    return 0;
}

int power(int base, int powerRaised)
{
    if (powerRaised != 0)
        return (base * power(base, powerRaised - 1));
    else
        return 1;
}