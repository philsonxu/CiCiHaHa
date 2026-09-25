#include <stdio.h>

/*
* C 语言实例 - 判断字母
* https://www.runoob.com/cprogramming/c-examples.html
*/
int main()
{
    char c;
    printf("输入一个字符: ");
    scanf("%c", &c);

    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
        printf("%c 是字母", c);
    else
        printf("%c 不是字母", c);

    return 0;
}