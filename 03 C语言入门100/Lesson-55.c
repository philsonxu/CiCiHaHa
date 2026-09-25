#include<stdio.h>

/*
* C 语言实例 - 删除字符串中的特殊字符
* https://www.runoob.com/cprogramming/c-examples.html
*/
int main()
{
    char line[150];
    int i, j;
    printf("输入一个字符串: ");
    fgets(line, (sizeof line / sizeof line[0]), stdin);

    for (i = 0; line[i] != '\0'; ++i)
    {
        while (!((line[i] >= 'a' && line[i] <= 'z') || (line[i] >= 'A' && line[i] <= 'Z') || line[i] == '\0'))
        {
            for (j = i; line[j] != '\0'; ++j)
            {
                line[j] = line[j + 1];
            }
            line[j] = '\0';
        }
    }
    printf("输出: ");
    puts(line);
    return 0;
}


int main()
{
    char line[100];
    int i, j, len;
    printf("输入一个字符串: ");
    scanf("%s", line);
    len = strlen(line);
    for (i = 0; i < len + 1; i++)
    {
        if ((line[i] >= 'a' && line[i] <= 'z') || (line[i] >= 'A' && line[i] <= 'Z'))
            continue;
        for (j = i; j < len; j++)
        {
            line[j] = line[j + 1];
        }
        len--;
        i--;
    }
    //line[len]='\0';
    printf("%s\n", line);
    return 0;
}