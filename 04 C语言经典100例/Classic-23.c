#include <stdio.h>

/*
* C 语言经典100例-23
* https://www.runoob.com/cprogramming/c-exercise-example23.html
* 题目：打印（菱形）图案。
*/
int main()
{
    int i, j, k;
    for (i = 0; i <= 3; i++) {
        for (j = 0; j <= 2 - i; j++) {
            printf(" ");
        }
        for (k = 0; k <= 2 * i; k++) {
            printf("*");
        }
        printf("\n");
    }
    for (i = 0; i <= 2; i++) {
        for (j = 0; j <= i; j++) {
            printf(" ");
        }
        for (k = 0; k <= 4 - 2 * i; k++) {
            printf("*");
        }
        printf("\n");
    }

    return 0;
}