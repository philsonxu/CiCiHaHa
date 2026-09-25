#include <stdio.h>

/*
* C 语言经典100例-08
* https://www.runoob.com/cprogramming/c-exercise-example8.html
* 题目：输出 9*9 口诀。
*/
int main()
{
    int i, j, result;
    printf("\n");
    for (i = 1; i < 10; i++) {
        for (j = 1; j <= i; j++) {
            result = i * j;
            printf("%d*%d=%-3d", i, j, result); /* -3d表示左对齐，占3位 */
        }
        printf("\n"); /* 每一行后换行 */
    }
    return 0;
}