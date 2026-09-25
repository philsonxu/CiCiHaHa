/*
 * 例8.9 返回指针值的函数（指针函数）
 * 知识点：函数返回类型为指针，如 int* func()
 */
#include <stdio.h>
#include <string.h>

/* 在字符串中查找指定字符，返回该字符第一次出现的位置指针 */
char* my_strchr(char *str, char ch)
{
    while (*str != '\0') {
        if (*str == ch)
            return str;
        str++;
    }
    return NULL;  /* 没找到 */
}

int main()
{
    char str[] = "Hello World";
    char ch = 'W';
    char *p;
    
    p = my_strchr(str, ch);
    if (p != NULL)
        printf("字符'%c'第一次出现位置后面的字符串：%s\n", ch, p);
    else
        printf("未找到字符'%c'\n", ch);
    
    return 0;
}