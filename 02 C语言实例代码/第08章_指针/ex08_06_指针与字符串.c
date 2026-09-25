/*
 * 例8.6 字符指针与字符串
 * 知识点：字符指针指向字符串常量、指针操作字符串
 */
#include <stdio.h>

int main()
{
    /* 字符指针指向字符串 */
    char *str = "Hello World";
    printf("字符串：%s\n", str);
    
    /* 用指针遍历字符串 */
    char *p;
    printf("逐个字符输出：");
    for (p = str; *p != '\0'; p++) {
        putchar(*p);
    }
    putchar('\n');
    
    /* 字符串复制：用指针实现 */
    char src[] = "I love C";
    char dest[20];
    char *ps = src, *pd = dest;
    while ((*pd++ = *ps++) != '\0')
        ;  /* 空循环体 */
    printf("复制后dest：%s\n", dest);
    
    return 0;
}