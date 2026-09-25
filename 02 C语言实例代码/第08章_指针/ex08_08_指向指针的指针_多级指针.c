/*
 * 例8.8 指向指针的指针（二级指针）
 * 知识点：**pp 存储指针变量的地址
 */
#include <stdio.h>

int main()
{
    int a = 10;
    int *p = &a;    /* 一级指针 */
    int **pp = &p;  /* 二级指针，指向p */
    
    printf("a = %d\n", a);
    printf("*p = %d\n", *p);
    printf("**pp = %d\n", **pp);
    
    /* 通过二级指针修改a */
    **pp = 100;
    printf("修改后 a = %d\n", a);
    
    return 0;
}