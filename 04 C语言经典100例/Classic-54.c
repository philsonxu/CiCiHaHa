#include <stdio.h>

void print_binary(int num);

/*
* C 语言经典100例-54
* https://www.runoob.com/cprogramming/c-exercise-example54.html
* 题目：按位 取反~ 运算符
*/
int main()
{
    int a, b;
    a = 234;
    b = ~a;
    //printf("a 的按位取反值为（十进制） %d \n", b);
    print_binary(a);
    print_binary(b);
    a = ~b;
    //printf("a 的按位取反值为（十六进制） %x \n", a);
    print_binary(a);

    return 0;
}