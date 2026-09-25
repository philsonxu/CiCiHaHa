#include <stdio.h>

/*
* C 语言经典100例-14
* https://www.w3cschool.cn/c/c-exercise-example14.html
* 题目：将一个正整数分解质因数。
* 例如：输入90,打印出90=2*3*3*5。
* 程序分析：对n进行分解质因数，应先找到一个最小的质数k，然后按下述步骤完成：
* (1)如果这个质数恰等于（小于的时候，继续执行循环）n，则说明分解质因数的过程已经结束，另外 打印出即可。
* (2)但n能被k整除，则应打印出k的值，并用n除以k的商,作为新的正整数n.重复执行第二步。
* (3)如果n不能被k整除，则用k+1作为k的值,重复执行第一步。
*/
int main()
{
    int n, i;
    printf("正整数分解质因数\n");
    printf("请输入整数：");
    scanf("%d", &n);
    printf("%d=", n);
    for (i = 2; i <= n; i++)
    {
        while (n % i == 0)
        {
            printf("%d", i);
            n /= i;
            if (n != 1) printf("*");
        }
    }

    printf("\n");
    return 0;
}