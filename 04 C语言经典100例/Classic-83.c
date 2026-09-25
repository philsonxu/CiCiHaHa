#include <stdio.h>

/*
* C 语言经典100例-83
* https://www.runoob.com/cprogramming/c-exercise-example83.html
* 题目：求0—7所能组成的奇数个数
*/
int main(int agrc, char* agrv[])
{
    //sum的初始值为4表示，只有一位数字组成的奇数个数为4个
    long sum = 4, s = 4;
    int j;
    for (j = 2; j <= 8; j++)
    {
        printf("%d位数为奇数的个数%ld\n", j - 1, s);
        if (j <= 2)
            s *= 7;
        else
            s *= 8;
        sum += s;
    }
    printf("%d位数为奇数的个数%ld\n", j - 1, s);
    printf("奇数的总个数为：%ld\n", sum);
    // system("pause");
    return 0;
}