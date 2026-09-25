#include <stdio.h>

/*
* C 语言实例 - 判断素数
* https://www.runoob.com/cprogramming/c-examples.html
*/
int main()
{
    int n, i, flag = 0;

    printf("输入一个正整数: ");
    scanf("%d", &n);

    for (i = 2; i <= n / 2; ++i) {
        // 符合该条件不是素数
        if (n % i == 0) {
            flag = 1;
            break;
        }
    }
    if (n <= 1) {
        flag = 1; // 1 和 0 不是素数
    }
    if (flag == 0)
        printf("%d 是素数", n);
    else
        printf("%d 不是素数", n);

    return 0;
}

/*======================================================================*/

int main()
{
    int low, high, i, flag;
    printf("输入两个整数: ");
    scanf("%d %d", &low, &high);

    printf("%d 与 %d 之间的素数为: ", low, high);

    while (low < high)
    {
        flag = 0;

        for (i = 2; i <= low / 2; ++i)
        {
            if (low % i == 0)
            {
                flag = 1;
                break;
            }
        }

        if (flag == 0)
            printf("%d ", low);

        ++low;
    }

    return 0;
}

/*======================================================================*/


int checkPrimeNumber(int n);

int main()
{
    int n1, n2, i, flag;

    printf("输入两个正整数: ");
    scanf("%d %d", &n1, &n2);
    printf("%d 和 %d 间的素数为: ", n1, n2);

    for (i = n1 + 1; i < n2; ++i)
    {
        // 判断是否为素数
        flag = checkPrimeNumber(i);

        if (flag == 1)
            printf("%d ", i);
    }
    return 0;
}

// 函数定义
int checkPrimeNumber(int n)
{
    int j, flag = 1;

    for (j = 2; j <= n / 2; ++j)
    {
        if (n % j == 0)
        {
            flag = 0;
            break;
        }
    }
    return flag;
}

/*======================================================================*/

int issushu(int k);

int main()
{
    int n, count = 0;
    scanf("%d", &n);
    for (int i = n; i > 3; i--)
        if (issushu(i))
            if (issushu(i - 2))
                count++;
    printf("%d", count);
    return 0;
}

int issushu(int k)
{
    for (int i = 2; i < sqrt(k) + 1; i++) {
        if (k % i == 0)
            return 0;
    }
    return 1;
}
