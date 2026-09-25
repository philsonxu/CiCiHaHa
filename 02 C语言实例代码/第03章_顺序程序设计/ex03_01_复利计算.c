/*
 * 例3.1 复利计算
 * 知识点：浮点型变量、数学函数pow、#include <math.h>
 */
#include <stdio.h>
#include <math.h>

int main()
{
    float p, r, n;
    r = 0.1;   /* 年利率10% */
    n = 10;    /* 存款10年 */
    p = pow(1 + r, n);
    printf("p=%f\n", p);
    return 0;
}