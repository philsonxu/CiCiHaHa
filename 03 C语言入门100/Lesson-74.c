#include <stdio.h>

/*
* C 语言实例 - 字符串输入输出
* https://www.runoob.com/cprogramming/c-examples.html
*/
int main()
{
    char str[100];
    printf("Enter a string: ");
    fgets(str, sizeof(str), stdin);
    printf("You entered: %s", str);
    return 0;
}