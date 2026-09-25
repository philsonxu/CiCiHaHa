/*
 * 例9.3 指向结构体的指针
 * 知识点：结构体指针、->成员访问运算符
 */
#include <stdio.h>

struct Student {
    int num;
    char name[20];
    float score;
};

int main()
{
    struct Student stu = {1001, "Zhang San", 95.5f};
    struct Student *p = &stu;
    
    /* 三种访问方式等价 */
    printf("方式1（结构体变量.成员）：%s\n", stu.name);
    printf("方式2（(*p).成员）：%s\n", (*p).name);
    printf("方式3（p->成员）：%s\n", p->name);
    
    printf("\n完整信息：\n");
    printf("学号：%d\n", p->num);
    printf("姓名：%s\n", p->name);
    printf("成绩：%.1f\n", p->score);
    
    return 0;
}