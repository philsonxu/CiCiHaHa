#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 10

/*
* C 语言经典100例-40
* https://www.runoob.com/cprogramming/c-exercise-example40.html
* 题目：数组逆序输出
*/
int main()
{
    int a[N] = { 0,1,2,3,4,5,6,7,8,9 };
    int i, t;
    printf("数组逆序输出\n");
    printf("原始数组是:\n");
    for (i = 0; i < N; i++)
        printf("%d ", a[i]);
    for (i = 0; i < N / 2; i++)
    {
        t = a[i];
        a[i] = a[N - 1 - i];
        a[N - 1 - i] = t;
    }
    printf("\n逆序后的数组:\n");
    for (i = 0; i < N; i++)
        printf("%d ", a[i]);
    printf("\n");
    return 0;
}


// 交换两个变量的值
void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}


int main()
{
    srand((unsigned int)time(NULL));
    int a[N];
    int i;
    printf("数组逆序输出\n");
    printf("原始数组是:\n");
    for (i = 0; i < N; i++)
    {
        a[i] = rand() % 100;
        printf("%d ", a[i]);
    }
    for (i = 0; i < N / 2; i++)
    {
        swap(&a[i], &a[N - 1 - i]);
    }
    printf("\n逆序后的数组:\n");
    for (i = 0; i < N; i++)
        printf("%d ", a[i]);
    printf("\n");

    return 0;
}