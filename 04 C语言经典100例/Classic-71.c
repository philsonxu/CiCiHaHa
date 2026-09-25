#include<stdio.h>
#include<stdlib.h>

typedef struct {
    char name[20];
    char sex[5];
    int  age;
} student;

void input(student* stu);
void output(student* stu);

void input(student* stu)
{
    int i;
    for (i = 0; i < 5; i++)
        scanf("%s %s %d", stu[i].name, stu[i].sex, &(stu[i].age));
}

void output(student* stu)
{
    int i;
    for (i = 0; i < 5; i++)
        printf("%s %s %d\n", stu[i].name, stu[i].sex, stu[i].age);
}

/*
* C 语言经典100例-71
* https://www.runoob.com/cprogramming/c-exercise-example71.html
* 题目：结构体struct保存信息
* 编写input()和output()函数输入，输出5个学生的数据记录。
*/
int main()
{
    student stu[5];
    printf("请输入5个学生的信息：姓名 性别 年龄:\n");
    input(stu);
    printf("5个学生的信息如下：\n姓名  性别  年龄\n");
    output(stu);

    //system("pause");
    return 0;
}