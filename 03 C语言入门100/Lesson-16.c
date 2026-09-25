#include <stdio.h>

/*
* C 语言实例 - 判断三个数中的最大数
* https://www.runoob.com/cprogramming/c-examples.html
*/
int main()
{
    double n1, n2, n3;

    printf("请输入三个数，以空格分隔: ");
    scanf("%lf %lf %lf", &n1, &n2, &n3);

    if (n1 >= n2 && n1 >= n3)
        printf("%.2f 是最大数。", n1);

    if (n2 >= n1 && n2 >= n3)
        printf("%.2f 是最大数。", n2);

    if (n3 >= n1 && n3 >= n2)
        printf("%.2f 是最大数。", n3);

    return 0;
}