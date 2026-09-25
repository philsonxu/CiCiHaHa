#include<stdio.h>

struct student
{
    int x;
    char c;
} a;

void f1(struct student b)
{
    b.x = 20;
    b.c = 'y';
}

void f2(struct student* b)
{
    b->x = 20;
    b->c = 'y';
}

/*
* C 语言经典100例-87
* https://www.runoob.com/cprogramming/c-exercise-example87.html
* 题目：结构体变量传递
*/
int main()
{
    printf("结构体变量传递\n");
    a.x = 3;
    a.c = 'a';
    printf("初始值\n");
    printf("%d,%c\n", a.x, a.c);

    printf("未用指针\n");
    f1(a);
    printf("%d,%c\n", a.x, a.c);

    printf("用指针\n");
    f2(&a);
    printf("%d,%c\n", a.x, a.c);

    return 0;
}
