#include <stdio.h>

/*
* C 语言实例 - 查找数组中最小的元素
* https://www.runoob.com/cprogramming/c-examples.html
*/
int main()
{
    int array[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 };
    int loop, smallest;

    smallest = array[0];

    for (loop = 1; loop < 10; loop++) {
        if (smallest > array[loop])
            smallest = array[loop];
    }

    printf("最小元素为 %d", smallest);

    return 0;
}