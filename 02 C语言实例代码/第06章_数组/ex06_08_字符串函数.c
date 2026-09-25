/*
 * 例6.8 常用字符串处理函数
 * 知识点：strlen/strcpy/strcat/strcmp函数、#include <string.h>
 */
#include <stdio.h>
#include <string.h>

int main()
{
    char str1[80] = "Hello";
    char str2[] = " World";
    char str3[80];
    
    /* strlen：求字符串长度 */
    printf("str1的长度：%d\n", strlen(str1));
    
    /* strcpy：字符串复制 */
    strcpy(str3, str1);
    printf("复制后str3：%s\n", str3);
    
    /* strcat：字符串连接 */
    strcat(str1, str2);
    printf("连接后str1：%s\n", str1);
    
    /* strcmp：字符串比较 */
    char a[] = "abc", b[] = "abd", c[] = "abc";
    printf("strcmp(\"abc\",\"abd\") = %d\n", strcmp(a, b));  /* 负数 */
    printf("strcmp(\"abd\",\"abc\") = %d\n", strcmp(b, a));  /* 正数 */
    printf("strcmp(\"abc\",\"abc\") = %d\n", strcmp(a, c));  /* 0 */
    
    /* strcmp用于判断字符串是否相等 */
    if (strcmp(a, c) == 0)
        printf("a和c相等\n");
    
    return 0;
}