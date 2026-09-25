/*
 * 例9.6 枚举类型 enum
 * 知识点：枚举常量默认从0开始递增，提高代码可读性
 */
#include <stdio.h>

/* 定义星期枚举 */
enum Weekday {
    MON = 1, TUE, WED, THU, FRI, SAT, SUN  /* 手动指定MON=1，后面依次递增 */
};

/* 定义颜色枚举 */
enum Color { RED, GREEN, BLUE };

int main()
{
    enum Weekday today;
    today = WED;
    printf("今天是星期%d\n", today);
    
    if (today == SAT || today == SUN)
        printf("今天是周末！\n");
    else
        printf("今天是工作日。\n");
    
    enum Color c = GREEN;
    switch (c) {
        case RED:   printf("颜色：红色\n"); break;
        case GREEN: printf("颜色：绿色\n"); break;
        case BLUE:  printf("颜色：蓝色\n"); break;
    }
    
    return 0;
}