/*
 * 例7.3 函数的嵌套调用
 * 知识点：函数间可以互相调用（不能嵌套定义）
 * 例：求四个整数中的最大值
 */
#include <stdio.h>

int max2(int x, int y)
{
    return (x > y) ? x : y;
}

int max4(int a, int b, int c, int d)
{
    int m;
    m = max2(a, b);
    m = max2(m, c);
    m = max2(m, d);
    return m;
}

int main()
{
    int a, b, c, d;
    printf("请输入4个整数：");
    scanf("%d,%d,%d,%d", &a, &b, &c, &d);
    printf("最大值是：%d\n", max4(a, b, c, d));
    return 0;
}