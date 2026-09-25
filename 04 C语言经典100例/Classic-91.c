#include <stdio.h>
#include <time.h>

/*
* C 语言经典100例-91
* https://www.runoob.com/cprogramming/c-exercise-example91.html
* 题目：时间函数1
*/
int main()
{
    time_t rawtime;
    struct tm* timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    printf("当前本地时间为: %s", asctime(timeinfo));

    return 0;
}