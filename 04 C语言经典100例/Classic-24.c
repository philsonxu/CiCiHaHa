#include <stdio.h>

/*
* C 语言经典100例-24
* https://www.runoob.com/cprogramming/c-exercise-example24.html
* 题目：分数序列之和
* 有一分数序列：2/1，3/2，5/3，8/5，13/8，21/13...求出这个数列的前20项之和。
*/
int main()
{
    printf("分数序列之和\n");
    int i;
    float t;
    float sum = 0;
    float a = 2, b = 1;
    for (i = 1; i <= 20; i++)
    {
        printf("%.0f/%.0f(%4f)", a, b, a / b);
        if (i < 20) printf(" + ");
        sum = sum + a / b;
        t = a;
        a = a + b;
        b = t;
    }
    printf(" = %9.6f\n", sum);

    return 0;
}