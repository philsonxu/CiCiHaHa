#include <stdio.h>

/*
* C 语言实例 - 查找字符在字符串中出现的次数
* https://www.runoob.com/cprogramming/c-examples.html
*/
int main()
{
    char str[1000], ch;
    int i, frequency = 0;

    printf("输入字符串: ");
    fgets(str, (sizeof str / sizeof str[0]), stdin);

    printf("输入要查找的字符: ");
    scanf("%c", &ch);

    for (i = 0; str[i] != '\0'; ++i)
    {
        if (ch == str[i])
            ++frequency;
    }

    printf("字符 %c 在字符串中出现的次数为 %d", ch, frequency);

    return 0;
}