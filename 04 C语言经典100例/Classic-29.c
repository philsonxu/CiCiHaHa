#include <stdio.h>

/*
* C 语言经典100例-29
* https://www.runoob.com/cprogramming/c-exercise-example29.html
* 题目：整数的位数与逆序
* 给一个不多于5位的正整数，要求：一、求它是几位数，二、逆序打印出各位数字。
*/
int main()
{
    long a, b, c, d, e, x;
    printf("整数的位数与逆序\n");
    printf("请输入 5 位数字：");
    scanf("%ld", &x);
    a = x / 10000;          /*分解出万位*/
    b = x % 10000 / 1000;   /*分解出千位*/
    c = x % 1000 / 100;     /*分解出百位*/
    d = x % 100 / 10;       /*分解出十位*/
    e = x % 10;             /*分解出个位*/
    if (a != 0) {
        printf("为 5 位数,逆序为： %ld %ld %ld %ld %ld\n", e, d, c, b, a);
    }
    else if (b != 0) {
        printf("为 4 位数,逆序为： %ld %ld %ld %ld\n", e, d, c, b);
    }
    else if (c != 0) {
        printf("为 3 位数,逆序为：%ld %ld %ld\n", e, d, c);
    }
    else if (d != 0) {
        printf("为 2 位数,逆序为： %ld %ld\n", e, d);
    }
    else if (e != 0) {
        printf("为 1 位数,逆序为：%ld\n", e);
    }

    return 0;
}