#include <stdio.h>

/*
* C 语言实例 - 判断回文数
* https://www.runoob.com/cprogramming/c-examples.html
*/
int main()
{
    int n, reversedInteger = 0, remainder, originalInteger;

    printf("输入一个整数: ");
    scanf("%d", &n);

    originalInteger = n;

    // 翻转
    while (n != 0)
    {
        remainder = n % 10;
        reversedInteger = reversedInteger * 10 + remainder;
        n /= 10;
    }

    // 判断
    if (originalInteger == reversedInteger)
        printf("%d 是回文数。", originalInteger);
    else
        printf("%d 不是回文数。", originalInteger);

    return 0;
}

int main()
{
    int a = 12021;
    char s[10] = { '\0' }, s1[10] = { '\0' };
    sprintf(s, "%d", a); // 将整数转换为字符串
    int n = strlen(s);
    int j = 0;
    for (int i = n - 1; i >= 0; i--)
    {
        s1[j++] = s[i];
    }
    //s[j]='\0';
    printf("%s %s\n", s, s1);
    if (!strcmp(s, s1))
        printf("整数%d是回文串", a);
    else
        printf("整数%d不是回文串", a);
    return 0;
}