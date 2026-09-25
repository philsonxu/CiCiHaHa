#include <stdio.h>

/*
* C 语言实例 - 判断数字为几位数
* https://www.runoob.com/cprogramming/c-examples.html
*/
int main()
{
    long long n;
    int count = 0;

    printf("输入一个整数: ");
    scanf("%lld", &n);

    while (n != 0)
    {
        // n = n/10
        n /= 10;
        ++count;
    }

    printf("数字是 %d 位数。", count);
}

int main()
{
    int a, b = 10, c = 1;
    printf("请输入一个人数字: ");
    scanf("%d", &a);
    while (a / b > 0)
    {
        b = b * 10;
        c++;
    };
    printf("%d是一个%d位数 \n", a, c);
    return 0;
}