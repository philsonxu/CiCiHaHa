#include <stdio.h>

#define true  1
#define false 0
#define bool unsigned char

/*
* C 语言经典100例-39
* https://www.runoob.com/cprogramming/c-exercise-example39.html
* 题目：数组插入问题
* 有一个已经排好序的数组。现输入一个数，要求按原来的规律将它插入数组中。
*/
int main()
{
    int a[11] = { 1,4,6,9,13,16,19,28,40,100 };
    int temp1, temp2, number, end, i, j;
    printf("数组插入问题\n");
    printf("原始数组是:\n");
    for (i = 0; i < 10; i++)
        printf("%4d", a[i]);
    printf("\n插入一个新的数字: ");
    scanf("%d", &number);
    end = a[9];
    if (number > end)
        a[10] = number;
    else
    {
        for (i = 0; i < 10; i++)
        {
            if (a[i] > number)
            {
                temp1 = a[i];
                a[i] = number;
                for (j = i + 1; j < 11; j++)
                {
                    temp2 = a[j];
                    a[j] = temp1;
                    temp1 = temp2;
                }
                break;
            }
        }
    }
    for (i = 0; i < 11; i++)
        printf("%4d", a[i]);
    printf("\n");

    return 0;
}

int main()
{
    int a[11] = { 2,4,6,9,13,16,19,28,40,100 };
    int i, number;
    printf("数组插入问题\n");
    printf("原始数组是:\n");
    for (i = 0; i < 10; i++)
        printf("%4d", a[i]);
    printf("\n插入一个新的数字: ");
    scanf("%d", &number);
    int b[11];
    int j = 0;
    bool inserted = false;
    for (int i = 0; i < 10; i++) {
        if (a[i] < number) {
            b[j++] = a[i];
        }
        else {
            if (inserted == false) {
                b[j++] = number;
                // 记录number是不是已经插入？
                inserted = true;
            }
            b[j++] = a[i];
        }
    }
    for (i = 0; i < 11; i++)
        printf("%4d", b[i]);
    printf("\n");

    return 0;
}