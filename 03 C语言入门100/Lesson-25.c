#include <stdio.h>

/*
* C 语言实例 - 求两数最小公倍数
* https://www.runoob.com/cprogramming/c-examples.html
*/
int main()
{
    int n1, n2, minMultiple;
    printf("输入两个正整数: ");
    scanf("%d %d", &n1, &n2);

    // 判断两数较大的值，并赋值给 minMultiple
    minMultiple = (n1 > n2) ? n1 : n2;

    // 条件为 true
    while (1)
    {
        if (minMultiple % n1 == 0 && minMultiple % n2 == 0)
        {
            printf("%d 和 %d 的最小公倍数为 %d", n1, n2, minMultiple);
            break;
        }
        ++minMultiple;
    }
    return 0;
}


int main()
{
    int n1, n2, i, gcd, lcm;

    printf("输入两个正整数: ");
    scanf("%d %d", &n1, &n2);

    for (i = 1; i <= n1 && i <= n2; ++i)
    {
        // 判断最大公约数
        if (n1 % i == 0 && n2 % i == 0)
            gcd = i;
    }

    lcm = (n1 * n2) / gcd;
    printf("%d 和 %d 的最小公倍数为 %d", n1, n2, lcm);

    return 0;
}