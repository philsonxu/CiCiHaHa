#include <stdio.h>

/*
* C 语言实例 - 连接字符串
* https://www.runoob.com/cprogramming/c-examples.html
*/
int main()
{
    char s1[100], s2[100], i, j;

    printf("输入第一个字符串: ");
    scanf("%s", s1);

    printf("输入第二个字符串: ");
    scanf("%s", s2);

    // 计算字符串 s1 长度
    for (i = 0; s1[i] != '\0'; ++i);

    for (j = 0; s2[j] != '\0'; ++j, ++i)
    {
        s1[i] = s2[j];
    }

    s1[i] = '\0';
    printf("连接后: %s", s1);

    return 0;
}

int main()
{
    char s1[100], s2[100];

    printf("输入第一个字符串: ");
    scanf("%s", s1);

    printf("输入第二个字符串: ");
    scanf("%s", s2);

    int len1 = strlen(s1);
    int len2 = strlen(s2);
    //printf("%d %d", len1, len2);
    for (int i = 0; i <= len2; i++)
    {
        s1[i + len1] = s2[i];
    }

    printf("%s\n", s1);
    // system("pause");
    return 0;
}

int main()
{
    char str1[100], str2[100], * p = str1, n = 0;
    printf("请输入第一个字符串:");
    scanf("%s", str1);
    printf("请输入第二个字符串:");
    scanf("%s", str2);
    while (*p++ != '\0');/*移动指针到str1尾*/
    --p;//回退一个单元，以便覆盖str1末的'\0p;//回退一个单元，以便覆盖str1末的'\0'
    while (str2[n] != '\0')
    {
        *p++ = str2[n];//将str2接到str1末
        ++n;
    };
    *p = '\0';//拼接完成，手动为str1末添上结束标记
    printf("结果为:\n%s\n\n", str1);
    return 0;
}