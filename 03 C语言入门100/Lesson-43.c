#include <stdio.h>
/*
* C 语言实例 - 输出数组
* https://www.runoob.com/cprogramming/c-examples.html
*/

int main()
{
    int array[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 };
    int loop;

    for (loop = 0; loop < 10; loop++)
        printf("%d ", array[loop]);

    return 0;
}

// 使用 for 循环逆向输出数组
int main()
{
    int array[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 };
    int loop;

    for (loop = 9; loop >= 0; loop--)
        printf("%d ", array[loop]);

    return 0;
}