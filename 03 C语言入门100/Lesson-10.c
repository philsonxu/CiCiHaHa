#include <stdio.h>
/*
* C 语言实例 - 数值比较
* https://www.runoob.com/cprogramming/c-examples.html
*/

int main()
{
    int a, b;

    a = 11;
    b = 99;

    // 也可以通过以下代码实现让用户在终端输入两个数
    // printf("输入第一个值:");
    // scanf("%d", &a);
    // printf("输入第二个值:");
    // scanf("%d", &b);

    if (a > b)
        printf("a 大于 b");
    else
        printf("a 小于等于 b");

    return 0;
}

int main()
{
    int a, b, c;

    a = 11;
    b = 22;
    c = 33;

    if (a > b && a > c)
        printf("%d 最大", a);
    else if (b > a && b > c)
        printf("%d 最大", b);
    else if (c > a && c > b)
        printf("%d 最大", c);
    else
        printf("有两个或三个数值相等");

    return 0;
}