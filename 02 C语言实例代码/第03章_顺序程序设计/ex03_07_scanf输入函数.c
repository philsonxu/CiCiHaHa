/*
 * 例3.7 scanf输入函数
 * 知识点：scanf格式、&取地址符、不同类型输入方法
 */
#include <stdio.h>

int main()
{
    int a, b;
    float f;
    char c;
    
    printf("请输入两个整数（用逗号分隔）：");
    scanf("%d,%d", &a, &b);
    printf("a=%d, b=%d\n", a, b);
    
    /* 清空输入缓冲区 */
    while(getchar() != '\n');
    
    printf("请输入一个浮点数和一个字符（空格分隔）：");
    scanf("%f %c", &f, &c);
    printf("f=%f, c=%c\n", f, c);
    
    return 0;
}