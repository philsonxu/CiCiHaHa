#include <stdio.h>

/*
* C 语言经典100例-80
* https://www.runoob.com/cprogramming/c-exercise-example80.html
* 题目：猴子分桃子
* 海滩上有一堆桃子，五只猴子来分。
* 第一只猴子把这堆桃子平均分为五份，多了一个，这只 猴子把多的一个扔入海中，拿走了一份。
* 第二只猴子把剩下的桃子又平均分成五份，又多了 一个，它同样把多的一个扔入海中，拿走了一份，
* 第三、第四、第五只猴子都是这样做的，
* 问海滩上原来最少有多少个桃子？
*/
int main()
{
    int n = 0;  // n 表示初始桃子数
    int i, count;
    int found = 0;

    printf("猴子分桃子问题：while循环\n");
    // 假设初始桃子数
    while (!found) {
        n += 1;
        count = n;
        for (i = 0; i < 5; i++) {
            if ((count - 1) % 5 == 0) {
                count = (count - 1) / 5 * 4;
            }
            else {
                break;
            }
        }
        if (i == 5) {
            found = 1;
        }
    }

    printf("最少有 %d 个桃子。\n", n);
    return 0;
}