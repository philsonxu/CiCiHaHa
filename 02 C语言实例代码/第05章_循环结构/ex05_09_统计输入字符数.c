/*
 * 例5.9 统计输入的各类字符个数
 * 知识点：while((c=getchar())!='\n')经典写法、字符分类判断
 */
#include <stdio.h>

int main()
{
    char c;
    int letters = 0, digits = 0, spaces = 0, others = 0;
    
    printf("请输入一行字符：\n");
    while ((c = getchar()) != '\n') {
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
            letters++;
        else if (c >= '0' && c <= '9')
            digits++;
        else if (c == ' ')
            spaces++;
        else
            others++;
    }
    
    printf("字母：%d个\n", letters);
    printf("数字：%d个\n", digits);
    printf("空格：%d个\n", spaces);
    printf("其他字符：%d个\n", others);
    return 0;
}