/*
 * 例3.5 强制类型转换
 * 知识点：(类型名)表达式、浮点转整型的截断规则
 */
#include <stdio.h>

int main()
{
    float f = 3.14159;
    int i;
    i = (int)f;      /* 强制转换为int，舍去小数部分 */
    printf("f = %f\n", f);
    printf("i = %d\n", i);
    
    /* 表达式中的类型转换 */
    int a = 5, b = 2;
    float div;
    div = (float)a / b;  /* 将a转为float，结果为浮点除法 */
    printf("%d / %d = %f\n", a, b, div);
    return 0;
}