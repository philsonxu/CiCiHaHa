#include <stdio.h>

#define TRUE 1
#define FALSE 0
#define SQ(x) (x)*(x)

/*
* C 语言经典100例-46
* https://www.runoob.com/cprogramming/c-exercise-example46.html
* 题目：#define宏命令1
*/
int main()
{
    int num;
    int again = TRUE;
    printf("#define宏命令1\n");
    printf("计算平方数，如果值小于 50 程序将终止。\n");
    while (again)
    {
        printf("\n请输入数字：");
        scanf("%d", &num);
        printf("该数字的平方为 %d \n", SQ(num));
        if (num >= 50)
            again = TRUE;
        else
            again = FALSE;
    }
    return 0;
}