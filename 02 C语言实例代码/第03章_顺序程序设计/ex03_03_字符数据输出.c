/*
 * 例3.3 字符数据的输出
 * 知识点：char类型、字符与ASCII码、%c与%d格式符
 */
#include <stdio.h>

int main()
{
    char c1 = 'a', c2 = 'b', c3 = 'c';
    printf("字符形式：%c%c%c\n", c1, c2, c3);
    printf("ASCII码值：%d,%d,%d\n", c1, c2, c3);
    
    /* 字符型数据与整型数据通用 */
    char c4 = 97, c5 = 98;
    printf("用整数赋值输出字符：%c %c\n", c4, c5);
    printf("输出整数：%d %d\n", c4, c5);
    return 0;
}