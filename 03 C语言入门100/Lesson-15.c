#include <stdio.h>

/*
* C 语言实例 - 判断元音/辅音
* https://www.runoob.com/cprogramming/c-examples.html
*/
int main()
{
    char c;
    int isLowercaseVowel, isUppercaseVowel;

    printf("输入一个字母: ");
    scanf("%c", &c);

    // 小写字母元音
    isLowercaseVowel = (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u');

    // 大写字母元音
    isUppercaseVowel = (c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U');

    // if 语句判断
    if (isLowercaseVowel || isUppercaseVowel)
        printf("%c 是元音", c);
    else
        printf("%c 是辅音", c);

    return 0;
}