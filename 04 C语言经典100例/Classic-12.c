#include <stdio.h>

/*
* C 语言经典100例-12
* https://www.w3cschool.cn/c/c-exercise-example12.html
* 题目：判断2到1000之间的素数。
* 程序分析：判断素数的方法：用一个数分别去除2到sqrt(这个数)，如果能被整除，则表明此数不是素数，反之是素数。
*/
int main()
{
    int i, j;
    int count = 0;

    printf("2...1000之间的质数\n");
    for (i = 2; i <= 1000; i++)
    {
        for (j = 2; j < i; j++)
        {
            // 如果j能被i整出在跳出循环
            if (i % j == 0)
                break;
        }
        // 判断循环是否提前跳出，如果j<i说明在2~j之间,i有可整出的数
        if (j >= i)
        {
            count++;
            printf("%3d ", i);
            // 换行，用count计数，每20个数换行
            if (count % 20 == 0)
                printf("\n");
        }
    }
    return 0;
}