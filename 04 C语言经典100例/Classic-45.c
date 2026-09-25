#include <stdio.h>

/*
* C 语言经典100例-45
* https://www.runoob.com/cprogramming/c-exercise-example45.html
* 题目：register变量
*/
int main()
{
    printf("register变量\n");
    register int i;
    int sum = 0;
    for (i = 1; i <= 100; i++)
        sum += i;
    printf("1+...+100 = %d\n", sum);
    return 0;
}


int sum(int a, int b)
{
    register int i;
    int sum = 0;
    for (i = a; i <= b; i++)
        sum += i;
    return sum;
}


int main()
{
    printf("register变量\n");
    int a = 1, b = 100;
    printf("%d+...+%d = %d\n", a, b, sum(a, b));
    return 0;
}