/*
 * 例8.1 指针变量的基本使用
 * 知识点：地址、指针的概念、&取地址、*解引用
 */
#include <stdio.h>

int main()
{
    int a = 10;
    int *p;       /* 定义指向int类型的指针变量p */
    p = &a;       /* p指向a，p存储a的地址 */
    
    printf("a的值：%d\n", a);
    printf("a的地址：%p\n", &a);
    printf("p的值（即a的地址）：%p\n", p);
    printf("p指向的值（*p）：%d\n", *p);
    
    /* 通过指针修改变量值 */
    *p = 20;
    printf("修改后a的值：%d\n", a);
    
    return 0;
}