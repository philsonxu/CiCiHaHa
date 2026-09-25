#include <stdio.h>

/*
* C 语言实例 - 判断奇数/偶数
* https://www.runoob.com/cprogramming/c-examples.html
*/
int main()
{
    int number;

    printf("请输入一个整数: ");
    scanf("%d", &number);

    // 判断这个数除以 2 的余数
    if (number % 2 == 0)
        printf("%d 是偶数。", number);
    else
        printf("%d 是奇数。", number);

    return 0;
}

// 奇偶数判断其实有个更简单高效的办法，
// 整数在计算机中存储的都是二进制，奇数的最后一位必是1。
int main()
{
    int number;

    printf("请输入一个整数: ");
    scanf("%d", &number);

    // 判断这个数最后一位是1这为奇数
    if (number & 1)
        printf("%d 是奇数。", number);
    else
        printf("%d 是偶数。", number);

    return 0;
}