#include<stdio.h>

#define exchange(a,b) { int t;t=a;a=b;b=t;} //注意放在一行里

/*
* C 语言经典100例-47
* https://www.runoob.com/cprogramming/c-exercise-example47.html
* 题目：#define宏命令2
*/
int main()
{
    int x = 10;
    int y = 20;
    printf("#define宏命令2\n");
    printf("before swap x=%d; y=%d\n", x, y);
    exchange(x, y);
    printf("after swap x=%d; y=%d\n", x, y);
    return 0;
}