#include <stdio.h>
/*
* C 语言实例 - 计算数组元素之和
* https://www.runoob.com/cprogramming/c-examples.html
*/

int main()
{
    int array[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 };
    int sum, loop;

    sum = 0;

    for (loop = 9; loop >= 0; loop--) {
        sum = sum + array[loop];
    }

    printf("元素和为：%d", sum);

    return 0;
}