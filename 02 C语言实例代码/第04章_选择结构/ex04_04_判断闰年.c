/*
 * 例4.4 判断闰年
 * 知识点：逻辑运算符&& || !、复杂条件组合
 * 闰年条件：能被4整除但不能被100整除，或者能被400整除
 */
#include <stdio.h>

int main()
{
    int year;
    printf("请输入年份：");
    scanf("%d", &year);
    
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
        printf("%d年是闰年\n", year);
    else
        printf("%d年不是闰年\n", year);
    
    return 0;
}