#include <stdio.h>

/*
* C 语言经典100例-42
* https://www.runoob.com/cprogramming/c-exercise-example42.html
* 题目：auto定义变量的用法
*/
int main()
{
    printf("auto定义变量的用法\n");
    int i, num;
    num = 2;
    for (i = 0; i < 3; i++)
    {
        printf("num 变量为 %d \n", num);
        num++;
        {
            auto int num = 1;
            printf("内置模块 num 变量为 %d \n", num);
            num++;
        }
    }

    return 0;
}