#include <stdio.h>

/*
* C 语言经典100例-15
* https://www.runoob.com/cprogramming/c-exercise-example15.html
* 题目：条件运算符
* 利用条件运算符的嵌套来完成此题：学习成绩>=90分的同学用A表示，60-89分之间的用B表示，60分以下的用C表示。
*/
int main()
{
    int score;
    char grade;
    printf("按分数标记等级\n");
    printf("请输入分数：");
    scanf("%d", &score);
    grade = (score >= 90) ? 'A' : ((score >= 60) ? 'B' : 'C');
    printf("%c\n", grade);
    return 0;
}