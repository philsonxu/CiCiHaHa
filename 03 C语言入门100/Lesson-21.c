#include <stdio.h>

/*
* C 语言实例 - 计算自然数的和
* https://www.runoob.com/cprogramming/c-examples.html
*/
int main()
{
    int n, i, sum = 0;

    printf("输入一个正整数: ");
    scanf("%d", &n);

    for (i = 1; i <= n; ++i)
    {
        sum += i;   // sum = sum+i;
    }

    printf("Sum = %d", sum);

    return 0;
}


int main()
{
    int n, i, sum = 0;

    printf("输入一个正整数: ");
    scanf("%d", &n);

    i = 1;
    while (i <= n)
    {
        sum += i;
        ++i;
    }

    printf("Sum = %d", sum);

    return 0;
}

int addNumbers(int n);

int main()
{
    int num;
    printf("输入一个整数: ");
    scanf("%d", &num);
    printf("Sum = %d", addNumbers(num));
    return 0;
}

int addNumbers(int n)
{
    if (n != 0)
        return n + addNumbers(n - 1);
    else
        return n;
}

int main()
{
    int num; printf("请输入一个自然数：");
    while (scanf("%d", &num) == 1) {
        if (num >= 0) {
            printf("Sum = %d", (num + 1) * num / 2);
            break;
        }
        else {
            printf("输入的不是正整数"); break;
        }
    }
}