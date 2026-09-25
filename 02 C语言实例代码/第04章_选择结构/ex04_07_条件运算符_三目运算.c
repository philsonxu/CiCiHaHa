/*
 * 例4.7 条件运算符（三目运算符）?:
 * 知识点：条件表达式、用?:替代简单if-else
 */
#include <stdio.h>

int main()
{
    int a, b, max;
    printf("请输入两个整数：");
    scanf("%d,%d", &a, &b);
    
    /* 条件运算符：表达式1 ? 表达式2 : 表达式3 */
    max = (a > b) ? a : b;
    printf("最大值是：%d\n", max);
    
    /* 嵌套使用条件运算符 */
    int c = 50;
    int max3 = (a > b) ? (a > c ? a : c) : (b > c ? b : c);
    printf("三个数%d,%d,%d的最大值是：%d\n", a, b, c, max3);
    
    return 0;
}