#include <stdio.h>

#define LAG >
#define SMA <
#define EQ ==

/*
* C 语言经典100例-48
* https://www.runoob.com/cprogramming/c-exercise-example48.html
* 题目：#define宏命令3
*/
int main()
{
    int a, b;
    printf("#define宏命令3\n");
    printf("请输入两个数字：\n");
    scanf("%d %d", &a, &b);
    if (a LAG b)
        printf("%d 大于 %d \n", a, b);
    else if (a EQ b)
        printf("%d 等于 %d \n", a, b);
    else if (a SMA b)
        printf("%d 小于 %d \n", a, b);
    else
        printf("没有值。\n");

    return 0;
}