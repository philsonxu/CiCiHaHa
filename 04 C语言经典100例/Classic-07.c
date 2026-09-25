#include <stdio.h>
#include <windows.h>

/*
* C 语言经典100例-07
* https://www.runoob.com/cprogramming/c-exercise-example7.html
* 题目：输出特殊图案
* 请在c环境中运行，看一看，Very Beautiful!
*/
int main()
{
    SetConsoleOutputCP(437);

    char a = 176, b = 219;
    printf("%c%c%c%c%c\n", b, a, a, a, b);
    printf("%c%c%c%c%c\n", a, b, a, b, a);
    printf("%c%c%c%c%c\n", a, a, b, a, a);
    printf("%c%c%c%c%c\n", a, b, a, b, a);
    printf("%c%c%c%c%c\n", b, a, a, a, b);

    SetConsoleOutputCP(CP_UTF8);
    return 0;
}