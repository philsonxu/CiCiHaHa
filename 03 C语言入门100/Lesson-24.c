#include <stdio.h>

/*
* C 语言实例 - 求两数的最大公约数
* https://www.runoob.com/cprogramming/c-examples.html
*/
int main()
{
    int n1, n2, i, gcd;

    printf("输入两个正整数，以空格分隔: ");
    scanf("%d %d", &n1, &n2);

    for (i = 1; i <= n1 && i <= n2; ++i)
    {
        // 判断 i 是否为最大公约数
        if (n1 % i == 0 && n2 % i == 0)
            gcd = i;
    }

    printf("%d 和 %d 的最大公约数是 %d", n1, n2, gcd);

    return 0;
}

int main()
{
    int n1, n2;

    printf("输入两个数，以空格分隔: ");
    scanf("%d %d", &n1, &n2);

    while (n1 != n2)
    {
        if (n1 > n2)
            n1 -= n2;
        else
            n2 -= n1;
    }
    printf("GCD = %d", n1);

    return 0;
}


int main()
{
    int n1, n2;

    printf("输入两个数，以空格分隔: ");
    scanf("%d %d", &n1, &n2);

    // 如果输入的是负数，将其转换为正数
    n1 = (n1 > 0) ? n1 : -n1;
    n2 = (n2 > 0) ? n2 : -n2;

    while (n1 != n2)
    {
        if (n1 > n2)
            n1 -= n2;
        else
            n2 -= n1;
    }
    printf("GCD = %d", n1);

    return 0;
}

int hcf(int n1, int n2);
int main()
{
    int n1, n2;
    printf("输入两个正整数: ");
    scanf("%d %d", &n1, &n2);

    printf("%d 和 %d 的最大公约数为 %d", n1, n2, hcf(n1, n2));
    return 0;
}

int hcf(int n1, int n2)
{
    if (n2 != 0)
        return hcf(n2, n1 % n2);
    else
        return n1;
}


int main()
{
    int a, b;
    int t;
    scanf("%d %d", &a, &b);
    while (b != 0) {
        t = a % b;
        a = b;
        b = t;
        printf("a=%d b=%d t=%d\n", a, b, t);
    }
    printf("最大公约数是%d\n", a);
    return 0;
}

int main(void)
{
    int x, y, gcd;
    printf("请输入两个正整数：");
    scanf("%d%d", &x, &y);
    if (x < y)
        gcd = x;
    else
        gcd = y;
    while (x % gcd || y % gcd)
        gcd--;
    printf("gcd = %d\n", gcd);

    return 0;
}