#include <stdio.h>

/*
* C 语言实例 - 判断正数/负数/零
* https://www.runoob.com/cprogramming/c-examples.html
*/
int main()
{
    double number;

    printf("输入一个数字: ");
    scanf("%lf", &number);

    if (number <= 0.0)
    {
        if (number == 0.0)
            printf("你输入的是 0。");
        else
            printf("你输入的是负数。");
    }
    else
        printf("你输入的是正数。");
    return 0;
}


int main()
{
    double  number;
    printf("请输入一个数字: ");
    scanf("%lf", &number);
    if (number < 0.0)
        printf("你输入的是负数>_<\n");
    else if (number > 0.0)
        printf("你输入的是正数-.-\n");
    else
        printf("你输入的是零0.0\n");
}


int main()
{
    int number, num;//因为下面使用了>>,所以定义为整型
    scanf("%d", &number);
    num = number >> 31;
    if (num == 0) {
        if (number == 0) {
            printf("输入的是0");
        }
        else {
            printf("输入的是正数");
        }
    }
    else {
        printf("你输入的是负数");
    }
}
