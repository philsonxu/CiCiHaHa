#include <stdio.h>

int a, b, c;
void add()
{
    int a;
    a = 3;
    c = a + b;
}

/*
* C 语言经典100例-44
* https://www.runoob.com/cprogramming/c-exercise-example44.html
* 题目：调用外部函数
*/
int main()
{
    printf("调用外部函数\n");
    a = b = 4;
    add();
    printf("c 的值为 %d\n", c);
    return 0;
}