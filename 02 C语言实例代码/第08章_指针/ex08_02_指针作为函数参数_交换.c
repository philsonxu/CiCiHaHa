/*
 * 例8.2 指针作为函数参数 - 真正实现两数交换
 * 知识点：地址传递、通过指针修改实参变量
 */
#include <stdio.h>

void iswap(int *p1, int *p2)
{
    int t;
    t = *p1;
    *p1 = *p2;
    *p2 = t;
}

int main()
{
    int a = 3, b = 5;
    printf("交换前：a=%d, b=%d\n", a, b);
    iswap(&a, &b);  /* 传入地址 */
    printf("交换后：a=%d, b=%d\n", a, b);
    return 0;
}