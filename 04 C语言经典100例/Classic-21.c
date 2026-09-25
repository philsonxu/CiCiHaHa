#include <stdio.h>
#include <stdlib.h>

/*
* C 语言经典100例-21
* https://www.runoob.com/cprogramming/c-exercise-example21.html
* 题目：猴子吃桃问题
* 猴子第一天摘下若干个桃子，当即吃了一半，还不瘾，又多吃了一个
* 第二天早上又将剩下的桃子吃掉一半，又多吃了一个。以后每天早上都吃了前一天剩下
* 的一半零一个。到第10天早上想再吃时，见只剩下一个桃子了。求第一天共摘了多少。
*/
int main()
{
    printf("猴子吃桃问题\n");
    int day, x1 = 0, x2;
    day = 9;
    x2 = 1;
    while (day > 0) {
        printf("第%2d天，总数为 %d\n", day + 1, x2);
        x1 = (x2 + 1) * 2;  // 第一天的桃子数是第2天桃子数加1后的2倍
        x2 = x1;
        day--;
    }
    printf("第%2d天，总数为 %d\n", day + 1, x1);
    printf("总数为 %d\n", x1);

    return 0;
}