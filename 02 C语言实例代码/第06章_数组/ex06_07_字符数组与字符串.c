/*
 * 例6.7 字符数组与字符串
 * 知识点：字符串以'\0'结尾、字符数组初始化、字符串输入输出
 */
#include <stdio.h>

int main()
{
    /* 字符串初始化方式 */
    char str1[] = "Hello World";              /* 自动加'\0' */
    char str2[20] = "C Language";              /* 剩余空间补'\0' */
    char str3[] = {'H', 'e', 'l', 'l', 'o', '\0'};  /* 手动加结束符 */
    
    printf("str1: %s\n", str1);
    printf("str2: %s\n", str2);
    printf("str3: %s\n", str3);
    
    /* 字符串输入 */
    char name[20];
    printf("请输入你的名字：");
    scanf("%s", name);  /* 注意：name本身是地址，不用&；遇到空格截止 */
    printf("你好，%s！\n", name);
    
    return 0;
}