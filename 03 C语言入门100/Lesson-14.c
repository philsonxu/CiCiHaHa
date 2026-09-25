#include <stdio.h>

/*
* C 语言实例 - 循环区间范围内的奇数/偶数
* https://www.runoob.com/cprogramming/c-examples.html
*/
// 通过循环输出指定区间范围的偶数
int main()
{
    int i;

    for (i = 1; i <= 10; i++) {
        if (i % 2 == 0)
            printf(" %2d\n", i);
    }
    return 0;
}

// 通过循环输出指定区间范围的奇数
int main()
{
    int i;

    for (i = 1; i <= 10; i++) {
        if (i % 2 != 0)
            printf("%d\n", i);
    }
    return 0;
}