#include <stdio.h>

/*
* C 语言实例 - 表格形式输出数据
* https://www.runoob.com/cprogramming/c-examples.html
*/
// 将 1~100 的数据以 10x10 矩阵格式输出。
int main()
{
    int i, j, count;

    for (i = 1; i <= 10; i++) {
        for (j = i; j <= 100; j += 10)
            printf(" %3d", j);

        printf("\n");
    }

    return 0;
}

 // 等差数列输出 10x10 矩阵格式
int main()
{
    int i, j, count;
    int start, end;

    start = 2, end = 10;

    for (i = start; i <= end; i++) {
        count = i;

        for (j = 1; j <= 10; j++) {
            printf(" %3d", count * j);
        }

        printf("\n");
    }

    return 0;
}

// 乘法运算格式。

int main()
{
    int i, j, n;

    n = 3;
    j = 1;

    for (i = n; i <= (n * 10); i += n) {
        printf("%3d  x %2d  =  %3d\n", n, j, i);
        j++;
    }

    return 0;
}