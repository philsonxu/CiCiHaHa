#include <stdio.h>

/*
* C 语言实例 - 循环输出26个字母
* https://www.runoob.com/cprogramming/c-examples.html
*/
int main()
{
    char letter = 'A'; // ASCII码中'A'的值为65

    // 使用循环输出26个字母
    for (int i = 0; i < 26; i++) {
        printf("%c ", letter);
        letter++; // 将letter变量的值递增，得到下一个字母的ASCII码
    }

    printf("\n");

    return 0;
}

int main()
{
    char c;

    for (c = 'A'; c <= 'Z'; ++c)
        printf("%c ", c);

    return 0;
}

int main()
{
    char letter = 'a'; // ASCII码中'a'的值为97

    // 使用循环输出26个小写字母
    for (int i = 0; i < 26; i++) {
        printf("%c ", letter);
        letter++; // 将letter变量的值递增，得到下一个字母的ASCII码
    }

    printf("\n");

    return 0;
}

int main()
{
    char c;

    printf("输入 u 显示大写字母，输入 l 显示小写字母: ");
    scanf("%c", &c);

    if (c == 'U' || c == 'u')
    {
        for (c = 'A'; c <= 'Z'; ++c)
            printf("%c ", c);
    }
    else if (c == 'L' || c == 'l')
    {
        for (c = 'a'; c <= 'z'; ++c)
            printf("%c ", c);
    }
    else
        printf("Error! 输入非法字符。");
    return 0;
}


int main()
{
    int A = 65;
    int i;
    for (i = 1; i <= 26; i++) {
        printf("%c\n", A);
        A++;
    }
    return 0;
}

int main()
{
    int i, n;

ant:printf("输入0或1输出小写或大写:");
    scanf("%d", &i);
    if (i != 0 && i != 1) {
        printf("error！输入0或1 ！\n");
        goto ant;
    }
    else {
        if (i == 0)
        {
            printf("0 小写:");
            for (n = 97; n < 123; n++)
                printf("%c ", n);
            printf("\n");
        }

        if (i == 1) {
            printf("1 大写:");
            for (n = 65; n < 91; n++)
                printf("%c ", n);
            printf("\n");
        }
    }
}