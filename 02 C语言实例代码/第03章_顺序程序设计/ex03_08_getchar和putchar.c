/*
 * 例3.8 getchar和putchar字符输入输出
 * 知识点：字符输入输出函数、回显输入
 */
#include <stdio.h>

int main()
{
    char c;
    printf("请输入一个字符：");
    c = getchar();          /* 从键盘读取一个字符 */
    
    printf("你输入的字符是：");
    putchar(c);             /* 输出一个字符 */
    putchar('\n');
    
    /* 连续输入输出 */
    printf("请输入三个字符：");
    char c1 = getchar();
    char c2 = getchar();
    char c3 = getchar();
    putchar(c1);
    putchar(c2);
    putchar(c3);
    putchar('\n');
    
    return 0;
}