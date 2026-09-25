#include <stdio.h>

/*
* C 语言实例 - 数组拆分与合并
* https://www.runoob.com/cprogramming/c-examples.html
*/
int main()
{
    int array[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    int even[10], odd[10];
    int loop, e, d;

    e = d = 0;

    for (loop = 0; loop < 10; loop++) {
        if (array[loop] % 2 == 0) {
            even[e] = array[loop];
            e++;
        }
        else {
            odd[d] = array[loop];
            d++;
        }
    }

    printf(" 原始数组 -> ");

    for (loop = 0; loop < 10; loop++)
        printf(" %d", array[loop]);

    printf("\n 偶数 -> ");
    for (loop = 0; loop < e; loop++)
        printf(" %d", even[loop]);

    printf("\n 奇数 -> ");
    for (loop = 0; loop < d; loop++)
        printf(" %d", odd[loop]);

    return 0;
}


int main()
{
    int array[10];
    int even[5] = { 0, 2, 4, 6, 8 };
    int odd[5] = { 1, 3, 5, 7, 9 };

    int loop, index, e_len, o_len;

    e_len = o_len = 5;

    index = 0;

    for (loop = 0; loop < e_len; loop++) {
        array[index] = even[loop];
        index++;
    }

    for (loop = 0; loop < o_len; loop++) {
        array[index] = odd[loop];
        index++;
    }

    printf("\n偶数 -> ");

    for (loop = 0; loop < e_len; loop++)
        printf(" %d", even[loop]);
    printf("\n奇数  -> ");

    for (loop = 0; loop < o_len; loop++)
        printf(" %d", odd[loop]);

    printf("\n合并后 -> ");

    for (loop = 0; loop < 10; loop++)
        printf(" %d", array[loop]);

    return 0;
}