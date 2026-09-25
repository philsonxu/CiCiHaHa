/*
 * 例3.6 printf格式控制
 * 知识点：%d %f %c %s %x %o格式符、宽度控制、精度控制、左右对齐
 */
#include <stdio.h>

int main()
{
    int a = 123;
    float b = 123.456;
    char c = 'A';
    char str[] = "Hello";
    
    printf("整型输出：\n");
    printf("a=%d\n", a);
    printf("a=%5d\n", a);    /* 占5位宽度 */
    printf("a=%-5d 后面\n", a); /* 左对齐 */
    
    printf("\n浮点型输出：\n");
    printf("b=%f\n", b);
    printf("b=%8.2f\n", b);  /* 总宽8位，小数2位 */
    printf("b=%.2f\n", b);   /* 只控制小数位数 */
    
    printf("\n字符和字符串：\n");
    printf("c=%c\n", c);
    printf("str=%s\n", str);
    printf("str=%10s\n", str);
    printf("str=%-10s后面\n", str);
    
    printf("\n八进制和十六进制：\n");
    printf("a的八进制：%o\n", a);
    printf("a的十六进制：%x\n", a);
    return 0;
}