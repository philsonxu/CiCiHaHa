#include<stdio.h>
#include<stdlib.h>

/*
* C 语言经典100例-85
* https://www.runoob.com/cprogramming/c-exercise-example85.html
* 题目：判断一个素数能被几个9整除
*/
int main()
{
    int p, i;
    long int sum = 9;
    printf("判断一个素数能被几个9整除\n");
    printf("请输入一个素数:\n");
    scanf("%d", &p);
    for (i = 1;; i++)
        if (sum % p == 0) break;
        else sum = sum * 10 + 9;

    printf("素数 %d 能整除 %d 个9组成的数 %ld\n", p, i, sum);

    return 0;
}