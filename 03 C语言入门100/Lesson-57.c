#include <stdio.h>
#include <string.h>
/*
* C 语言实例 - 计算字符串长度
* https://www.runoob.com/cprogramming/c-examples.html
*/

int main()
{
    char s[1000];
    int len;

    printf("输入字符串: ");
    scanf("%s", s);
    len = strlen(s);

    printf("字符串长度: %d", len);
    return 0;
}


int main()
{
    char s[1000], i;

    printf("输入字符串: ");
    scanf("%s", s);

    for(i = 0; s[i] != '\0'; ++i);

    printf("字符串长度: %d", i);
    return 0;
}
