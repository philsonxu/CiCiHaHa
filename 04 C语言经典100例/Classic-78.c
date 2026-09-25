#include <stdio.h>
#include <stdlib.h>

// 定义结构体类型，用于存储人员的姓名和年龄
struct man {
    char name[20];
    int age;
} person[3] = { 
    {"li", 68}, 
    {"wang", 75},
    {"sun", 52} 
};

/*
* C 语言经典100例-78
* https://www.runoob.com/cprogramming/c-exercise-example78.html
* 题目：长者问题：结构体struct指针
* 找到年龄最大的人，并输出。请找出程序中有什么问题。
*/
int main()
{
    struct man* q = NULL; // 用于指向年龄最大的人员
    struct man* p = person; // 指向数组的起始地址
    int max_age = 0; // 存储最大年龄

    printf("长者问题：结构体struct指针\n");

    // 遍历数组，找到年龄最大的人员
    int i;
    for (i = 0; i < 3; i++) {
        if (p->age > max_age) { // 如果当前年龄大于 max_age
            max_age = p->age; // 更新 max_age
            q = p; // 将 q 指向当前人员
        }
        p++; // 移动到下一个人员
    }

    // 输出年龄最大的人员的姓名和年龄
    if (q != NULL) {
        printf("%s %d\n", q->name, q->age);
    }
    else {
        printf("没有找到人员信息。\n");
    }

    return 0;
}