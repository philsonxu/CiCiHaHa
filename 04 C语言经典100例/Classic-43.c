#include <stdio.h>

/*
* C 语言经典100例-43
* https://www.runoob.com/cprogramming/c-exercise-example43.html
* 题目：static的另一用法
*/
int main()
{
    printf("static的另一用法\n");
    int i, num;
    num = 2;
    for (i = 0; i < 3; i++)
    {
        printf("num 变量为 %d \n", num);
        num++;
        {
            static int num = 1;
            printf("内置模块 num 变量为 %d \n", num);
            num++;
        }
    }

    return 0;
}