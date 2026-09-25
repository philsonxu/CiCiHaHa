#include <stdio.h>

int age_question(int n)
{
    int c;
    if (n == 1) c = 10;
    else c = age_question(n - 1) + 2;
    //printf("%d\n", c);
    return(c);
}

/*
* C 语言经典100例-28
* https://www.runoob.com/cprogramming/c-exercise-example28.html
* 题目：5人的岁数问题
* 有5个人坐在一起，问第五个人多少岁？他说比第4个人大2岁。
* 问第4个人岁数，他说比第3个人大2岁。
* 问第3个人，又说比第2人大两岁。
* 问第2个人，说比第一个人大两岁。
* 最后问第一个人，他说是10岁。请问第五个人多大？
*/
int main()
{
    printf("5人的岁数问题\n");
    printf("%d\n", age_question(5));

    return 0;
}