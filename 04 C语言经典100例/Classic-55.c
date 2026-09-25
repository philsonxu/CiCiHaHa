#include <stdio.h>

void print_binary(int num);

/*
* C 语言经典100例-55
* https://www.runoob.com/cprogramming/c-exercise-example55.html
* 题目：整数从右端开始的 4～7 位。
*/
int main()
{
    int a, b, c, d;
    printf("整数从右端开始的 4～7 位\n");
    printf("请输入整数：\n");
    scanf("%d", &a);
    b = a >> 4;
    c = ~(~0 << 4);
    d = b & c;
    //printf("%d\n%d\n", a, d);

    printf("a:\n");
    print_binary(a);
    printf("b:\n");
    print_binary(b);
    printf("c:\n");
    print_binary(c);
    printf("d:\n");
    print_binary(d);

    return 0;
}