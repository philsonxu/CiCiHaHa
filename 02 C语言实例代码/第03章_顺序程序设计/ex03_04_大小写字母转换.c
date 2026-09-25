/*
 * 例3.4 大小写字母转换
 * 知识点：字符运算、ASCII码差值（大写+32=小写）、getchar/putchar
 */
#include <stdio.h>

int main()
{
    char c1, c2;
    c1 = 'A';
    c2 = 'B';
    c1 = c1 + 32;   /* 大写转小写 */
    c2 = c2 + 32;
    printf("%c%c\n", c1, c2);
    printf("%d,%d\n", c1, c2);
    return 0;
}