#include <stdio.h>

/*
void reverse(char* s)
{
    // 获取字符串长度
    int len = 0;
    char* p = s;
    while (*p != 0)
    {
        len++;
        p++;
    }

    // 交换 ...
    int i = 0;
    char c;
    while (i <= len / 2 - 1)
    {
        c = *(s + i);
        *(s + i) = *(s + len - 1 - i);
        *(s + len - 1 - i) = c;
        i++;
    }
}
*/
void strrevs(char* s);

/*
* C 语言经典100例-34
* https://www.runoob.com/cprogramming/c-exercise-example34.html
* 题目：字符串反转
* 如将字符串 "www.runoob.com" 反转为 "moc.boonur.www"。
*/
int main()
{
    printf("字符串反转\n");
    char s[] = "www.runoob.com";
    printf("'%s' =>\n", s);
    // 反转字符串
    strrevs(s);
    printf("'%s'\n", s);
    return 0;
}