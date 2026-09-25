#include <stdio.h>
/*
* C 语言实例 - 字符串翻转
* https://www.runoob.com/cprogramming/c-examples.html
*/
void reverseSentence();

int main()
{
    printf("输入一个字符串: ");
    reverseSentence();

    return 0;
}

void reverseSentence()
{
    char c;
    scanf("%c", &c);

    if( c != '\n')
    {
        reverseSentence();
        printf("%c",c);
    }
}


char* reverseStr(char* str);

int main()
{
    char str[30];
    printf("输入一个字符串: ");
    scanf("%s", str);
    printf("翻转之前的字符串为:%s\n", str);
    printf("翻转之后的字符串为:%s", reverseStr(str));
    return 0;
}

char* reverseStr(char* str)
{
    int i = 0;
    int j = strlen(str) - 1;
    char temp;
    while (i < j)
    {
        temp = *(str + i);
        *(str + i) = *(str + j);
        *(str + j) = temp;
        i++;
        j--;
    }
    return str;
}

void reverseStr_02(char* str);

int main()
{
    char str[30];
    printf("输入一个字符串: ");
    scanf("%s", str);
    printf("翻转之前的字符串为:");
    reverseStr_02(str);
    return 0;
}

void reverseStr_02(char* str)
{
    if (*str == '\0') return;
    reverseStr_02(str + 1);
    printf("%c", *str);
}

int main()
{
    char c[40];
    int i = 1;
    printf("请输入字符串：");
    scanf("%s", c);
    int j = strlen(c);
    do {
        printf("%c", c[j - i]);
        ++i;
    } while (i <= j);
    return 0;
}