#include <stdio.h>

/*
* C 语言实例 - 计算一个数是否可为两个素数之和
* https://www.runoob.com/cprogramming/c-examples.html
*/
int checkPrime(int n);

int main()
{
    int n, i, flag = 0;

    printf("计算一个数是否可为两个素数之和\n");
    printf("输入正整数: ");
    scanf("%d", &n);

    for (i = 2; i <= n / 2; ++i)
    {
        // 检测判断
        if (checkPrime(i) == 1)
        {
            if (checkPrime(n - i) == 1)
            {
                printf("%d = %d + %d\n", n, i, n - i);
                flag = 1;
            }

        }
    }

    if (flag == 0)
        printf("%d 不能分解为两个素数。", n);

    return 0;
}

// 判断素数
int checkPrime(int n)
{
    int i, isPrime = 1;

    for (i = 2; i <= n / 2; ++i)
    {
        if (n % i == 0)
        {
            isPrime = 0;
            break;
        }
    }

    return isPrime;
}

int main()
{
    int number, tag;
    scanf("%d", &number);
    int i, j, k, l;
    for (i = 2; i < number; i++) {
        tag = 1;
        for (j = 2; j <= i / 2; j++) {
            if (i % j == 0) {//只要有一个余0就不是素数
                tag = 0;
                break;
            }
        }
        if (tag == 1) {//在找到第一个素数的基础上，找后面的素数，相加 是否等于number
            for (k = i; k < number; k++) {
                tag = 1;
                for (l = 2; l <= k / 2; l++) {
                    if (k % l == 0) {
                        tag = 0;
                        break;
                    }
                }
                if (tag == 1 && number == (i + k)) {
                    printf("%d 由 %d和 %d两个素数组成\n", number, i, k);
                }
            }
        }
    }
    if (tag == 0)
        printf("结束。");
    return 0;
}