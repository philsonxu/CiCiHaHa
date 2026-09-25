#include <stdio.h>
#include <math.h>

/*
* C 语言经典100例-03
* https://www.runoob.com/cprogramming/c-exercise-example3.html
* 题目：一个整数，它加上100后是一个完全平方数，再加上168又是一个完全平方数，请问该数是多少？
*/
int main(void)
{
    printf("\n计算方法1：\n");
    int  i, j, m, n, x;
    for (i = 1; i < 168 / 2 + 1; i++)
    {
        if (168 % i == 0)
        {
            j = 168 / i;
            if (i > j && (i + j) % 2 == 0 && (i - j) % 2 == 0)
            {
                m = (i + j) / 2;
                n = (i - j) / 2;
                x = n * n - 100;
                if (x > 0) {
                    printf("%4d + 100 = %4d = %2d * %2d\n", x, x + 100, n, n);
                    printf("%4d + 268 = %4d = %2d * %2d\n", x, x + 268, m, m);
                }
            }
        }
    }
    return 0;
}

/*
* C 语言经典100例-03
* https://www.w3cschool.cn/c/c-exercise-example3.html
* 题目：一个整数，它加上100后是一个完全平方数，再加上168又是一个完全平方数，请问该数是多少？
*/
int main()
{
    printf("\n计算方法2：\n");
    long int i, x, y;
    for (i = 1; i < 100000; i++)
    {
        // x为加上100后开方后的结果
        x = (long int)sqrt(i + 100);
        // y为再加上268后开方后的结果
        y = (long int)sqrt(i + 268);
        if (x * x == i + 100 && y * y == i + 268)
        {
            // 如果一个数的平方根的平方等于该数，这说明此数是完全平方数
            //printf("%d %d %ld\n", x, y, i);
            printf("%4ld + 100 = %4ld = %2d * %2d\n", i, i + 100, x, x);
            printf("%4ld + 168 = %4ld = %2d * %2d\n", i, i + 268, y, y);
        }
    }

    return 0;
}