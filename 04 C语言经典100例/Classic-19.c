#include <stdio.h>

/*
* C 语言经典100例-19
* https://www.runoob.com/cprogramming/c-exercise-example19.html
* 题目：找出 1000 以内的所有完数
* 一个数如果恰好等于它的因子之和，这个数就称为"完数"，
* 例如 6=1＋2＋3 ，请编程找出 1000 以内的所有完数。
*/
int main()
{
    int i, j, sum;

    printf("1000以内的完数有：\n");

    for (i = 1; i <= 1000; i++) {
        sum = 0;  // 每次循环前重置sum

        // 找出i的所有真因子并求和
        for (j = 1; j < i; j++) {
            if (i % j == 0) {
                sum += j;
            }
        }

        // 如果和等于i本身，则是完数
        if (sum == i) {
            printf("%d = 1", i);  // 所有完数都包含1

            // 打印其他因子
            for (j = 2; j < i; j++) {
                if (i % j == 0) {
                    printf(" + %d", j);
                }
            }
            printf("\n");
        }
    }

    return 0;
}