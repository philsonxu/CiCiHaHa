/*
 * 例9.1 结构体(struct)的定义和使用
 * 知识点：结构体类型定义、结构体变量定义与初始化、成员访问.
 */
#include <stdio.h>
#include <string.h>

/* 定义学生结构体类型 */
struct Student {
    int num;         /* 学号 */
    char name[20];   /* 姓名 */
    char sex;        /* 性别 */
    int age;         /* 年龄 */
    float score;     /* 成绩 */
};

int main()
{
    /* 方式1：定义时初始化 */
    struct Student stu1 = {1001, "Zhang San", 'M', 20, 95.5f};
    
    /* 方式2：逐个成员赋值 */
    struct Student stu2;
    stu2.num = 1002;
    strcpy(stu2.name, "Li Si");
    stu2.sex = 'F';
    stu2.age = 19;
    stu2.score = 88.0f;
    
    /* 输出结构体 */
    printf("学号：%d\n姓名：%s\n性别：%c\n年龄：%d\n成绩：%.1f\n",
           stu1.num, stu1.name, stu1.sex, stu1.age, stu1.score);
    printf("------------------------\n");
    printf("学号：%d\n姓名：%s\n性别：%c\n年龄：%d\n成绩：%.1f\n",
           stu2.num, stu2.name, stu2.sex, stu2.age, stu2.score);
    
    return 0;
}