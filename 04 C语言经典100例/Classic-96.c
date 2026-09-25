#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
* C 语言经典100例-96
* https://www.runoob.com/cprogramming/c-exercise-example96.html
* 题目：子串出现的次数
*/
int main(void)
{
    int i, j, k, TLen, PLen, count = 0;
    char T[50], P[10];
    printf("请输入两个字符串，以回车隔开，母串在前，子串在后：\n");
    gets(T);
    gets(P);
    TLen = strlen(T);
    PLen = strlen(P);
    for (i = 0; i <= TLen - PLen; i++)
    {
        for (j = 0, k = i; j < PLen && P[j] == T[k]; j++, k++)
            ;
        if (j == PLen)count++;
    }
    printf("%d\n", count);
    //system("pause");
    return 0;
}