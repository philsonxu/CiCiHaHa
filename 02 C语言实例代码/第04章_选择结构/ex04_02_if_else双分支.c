/*
 * 例4.2 if-else双分支 - 判断奇偶数
 * 知识点：if-else结构、%取余运算符
 */
#include <stdio.h>

int main()
{
    int num;
    printf("请输入一个整数：");
    scanf("%d", &num);
    
    if (num % 2 == 0)
        printf("%d 是偶数\n", num);
    else
        printf("%d 是奇数\n", num);
    
    return 0;
}