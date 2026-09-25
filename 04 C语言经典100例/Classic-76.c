#include <stdio.h>
#include <stdlib.h>

double  even_fraction_sum(int n);
double  odd_fraction_sum(int n);

/*
* C 语言经典100例-76
* https://www.runoob.com/cprogramming/c-exercise-example76.html
* 题目：函数指针，奇偶分数求和
* 编写一个函数，输入n为偶数时，调用函数求1/2+1/4+...+1/n,当输入n为奇数时，调用函数1/1+1/3+...+1/n(利用指针函数)。
*/
int main()
{
    int n;
    double r;
    double (*pfunc)(int);
    printf("函数指针，奇偶分数求和\n");
    printf("请输入一个数字：");
    scanf("%d", &n);
    if (n % 2 == 0) pfunc = even_fraction_sum;
    else pfunc = odd_fraction_sum;

    r = (*pfunc)(n);
    printf("%lf\n", r);

    //system("pause");
    return 0;
}

double  even_fraction_sum(int n)
{
    double s = 0, a = 0;
    int i;
    for (i = 2; i <= n; i += 2)
    {
        a = (double)1 / i;
        s += a;
    }
    return s;
}

double  odd_fraction_sum(int n)
{
    double s = 0, a = 0;
    int i;
    for (i = 1; i <= n; i += 2)
    {
        a = (double)1 / i;
        s += a;
    }
    return s;
}