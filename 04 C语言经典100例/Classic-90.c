#include<stdio.h>
#include<stdlib.h>

#define M 5

/*
* C 语言经典100例-90
* https://www.runoob.com/cprogramming/c-exercise-example90.html
* 题目：数组倒序问题
*/
int main()
{
    printf("数组与指针，数组倒序问题\n");
    int a[M] = { 1,2,3,4,5 };
    int i, j, t;
    i = 0; j = M - 1;
    while (i < j)
    {
        t = *(a + i);
        *(a + i) = *(a + j);
        *(a + j) = t;
        i++; j--;
    }
    for (i = 0; i < M; i++) {
        printf("%d\n", *(a + i));
    }

    return 0;
}