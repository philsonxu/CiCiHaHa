#include <stdio.h>
#include <time.h>
#include <math.h>

/*
* C 语言经典100例-92
* https://www.runoob.com/cprogramming/c-exercise-example92.html
* 题目：时间函数2
*/
int main()
{
    time_t start, end;
    int i;
    start = time(NULL);
    // 返回两个time_t型变量之间的时间间隔
    for (i = 0; i < 300000; i++)
    {
        //printf("\n");
        int j = 0;
        double k = 10;
        while (j < 2000) { k = sqrt(k); j++; }
    }
    end = time(NULL);

    // 输出执行时间
    printf("时间间隔为 %6.3f\n", difftime(end, start));

    return 0;
}