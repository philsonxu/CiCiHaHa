#include <stdio.h>
#include <string.h>

/*
* C 语言实例 - 字符串复制
* https://www.runoob.com/cprogramming/c-examples.html
*/

int main()
{
    char src[40];
    char dest[100];

    memset(dest, '\0', sizeof(dest));
    strcpy(src, "This is runoob.com");
    strcpy(dest, src);

    printf("最终的目标字符串： %s\n", dest);

    return(0);
}


int main()
{
    char s1[100], s2[100], i;

    printf("字符串 s1: ");
    scanf("%s", s1);

    for (i = 0; s1[i] != '\0'; ++i)
    {
        s2[i] = s1[i];
    }

    s2[i] = '\0';
    printf("字符串 s2: %s", s2);

    return 0;
}
