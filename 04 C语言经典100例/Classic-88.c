#include <stdio.h>
#include <stdlib.h>

/*
* C 语言经典100例-88
* https://www.runoob.com/cprogramming/c-exercise-example88.html
* 题目：整数的个数问题
* 读取7个数（1—50）的整数值，每读取一个值，程序打印出该值个数的 ＊。
*/
int main()
{
    int n, i = 1, j;
    printf("整数的个数问题\n");
    printf("请输入数字:\n");
    i--;
    for (i = 0; i < 7; i++)
    {
        scanf("%d", &n);
        if (n > 50) {
            printf("请重新输入:\n"); i--;
        }
        else
        {
            for (j = 0; j < n; j++)
                printf("*");
        }
        printf("\n");
    }
    return 0;
}