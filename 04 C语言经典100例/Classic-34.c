#include <stdio.h>

void hello_world(void)
{
    printf("Hello, world!\n");
}

void three_hellos(void)
{
    int counter;
    for (counter = 1; counter <= 3; counter++)
        hello_world();/*调用此函数*/
}

/*
* C 语言经典100例-34
* https://www.runoob.com/cprogramming/c-exercise-example34.html
* 题目：函数调用练习。
*/
int main(void)
{
    three_hellos();/*调用此函数*/

    return 0;
}