#include <stdio.h>

/*
* C 语言经典100例-27
* https://www.runoob.com/cprogramming/c-exercise-example27.html
* 题目：字符串反序
* 利用递归函数调用方式，将所输入的5个字符，以相反顺序打印出来。
*/
int main()
{
    int i = 5;
    void palin(int n);
    printf("字符串反序\n");
    printf("请输入5个字符: ");
    palin(i);
    printf("\n");

    return 0;
}

void palin(n)
int n;
{
    char next;
    if (n <= 1) {
        next = getchar();
        printf("相反顺序输出结果: ");
        putchar(next);
    }
    else {
        next = getchar();
        palin(n - 1);
        putchar(next);
    }
}