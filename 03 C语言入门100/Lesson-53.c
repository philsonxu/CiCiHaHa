#include<stdio.h>
/*
* C 语言实例 - 使用引用循环替换数值
* https://www.runoob.com/cprogramming/c-examples.html
*/

void cyclicSwap(int *a,int *b,int *c);

int main()
{
    int a, b, c;

    printf("输入 a, b 和 c 的值: ");
    scanf("%d %d %d", &a, &b, &c);

    printf("交换前:\n");
    printf("a = %d \nb = %d \nc = %d\n", a, b, c);

    cyclicSwap(&a, &b, &c);

    printf("交换后:\n");
    printf("a = %d \nb = %d \nc = %d", a, b, c);

    return 0;
}

void cyclicSwap(int* a, int* b, int* c)
{
    int temp;

    // 交换
    temp = *b;
    *b = *a;
    *a = *c;
    *c = temp;
}