#include <stdio.h>

/*
* C 语言经典100例-16
* https://www.runoob.com/cprogramming/c-exercise-example16.html
* 题目：输入两个正整数m和n，求其最大公约数和最小公倍数。
* 程序分析：
* （1）最小公倍数=输入的两个数之积除于它们的最大公约数，关键是求出最大公约数；
* （2）求最大公约数用辗转相除法（又名欧几里德算法）
* 第一步：a ÷ b，令r为所得余数；
* 第二步：互换：置 a←b，b←r，并返回第一步。
*/

int swap(int* a, int* b);

int main()
{
    int a, b, t, r, n;
    printf("求最大公约数和最小公倍数\n");
    printf("请输入两个数字：");
    scanf("%d %d", &a, &b);

    if (a < b)
    {
        //t = b; b = a; a = t;
        swap(&a, &b);
    }
    r = a % b;
    n = a * b;
    while (r != 0)
    {
        a = b;
        b = r;
        r = a % b;
    }
    printf("这两个数的最大公约数是%d，最小公倍数是%d\n", b, n / b);

    return 0;
}