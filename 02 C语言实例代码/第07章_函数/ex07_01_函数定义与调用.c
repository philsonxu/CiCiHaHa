/*
 * 例7.1 函数的定义和调用
 * 知识点：函数声明、定义、调用、参数传递、返回值
 */
#include <stdio.h>

/* 函数声明（原型声明） */
int max(int x, int y);

int main()
{
    int a, b, c;
    printf("请输入两个整数：");
    scanf("%d,%d", &a, &b);
    c = max(a, b);  /* 函数调用 */
    printf("最大值是：%d\n", c);
    return 0;
}

/* 函数定义：求两个整数的最大值 */
int max(int x, int y)
{
    return (x > y) ? x : y;
}