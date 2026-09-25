#include <stdio.h>

/*
* C 语言经典100例-56
* https://www.runoob.com/cprogramming/c-exercise-example61.html
* 题目：打印杨辉三角形
*/
int main()
{
    int i, j;
    int a[10][10];
    printf("打印杨辉三角形\n");
    for (i = 0; i < 10; i++) {
        a[i][0] = 1;
        a[i][i] = 1;
    }
    for (i = 2; i < 10; i++)
        for (j = 1; j < i; j++)
            a[i][j] = a[i - 1][j - 1] + a[i - 1][j];
    for (i = 0; i < 10; i++) {
        for (j = 0; j < (9 - i)*3; j++)
            printf(" ");
        for (j = 0; j <= i; j++)
            printf("%5d", a[i][j]);
        printf("\n");
    }

    return 0;
}