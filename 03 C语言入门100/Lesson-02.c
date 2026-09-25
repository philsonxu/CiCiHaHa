#include <stdio.h>
/*
* C 语言实例 - 输出整数
* https://www.runoob.com/cprogramming/c-examples.html
*/
int main()
{
    int number;

    // printf() 输出字符串
    printf("输入一个整数: ");  

    // scanf() 格式化输入
    scanf("%d", &number);  

    // printf() 显示格式化输入
    printf("你输入的整数是: %d", number);
    return 0;
}