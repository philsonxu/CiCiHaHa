/*
 * 例9.7 typedef 给类型起别名
 * 知识点：typedef简化复杂类型声明、提高可移植性
 */
#include <stdio.h>
#include <string.h>

/* 为基本类型起别名 */
typedef int Integer;
typedef float Real;

/* 为结构体起别名 */
typedef struct {
    char name[20];
    int age;
    float score;
} Student;  /* 以后直接用Student声明变量，不需写struct */

/* 为指针起别名 */
typedef char* String;

int main()
{
    Integer a = 10;
    Real b = 3.14f;
    printf("a = %d, b = %.2f\n", a, b);
    
    Student stu;
    strcpy(stu.name, "Zhang San");
    stu.age = 20;
    stu.score = 95.5f;
    printf("学生：%s, 年龄：%d, 成绩：%.1f\n", 
           stu.name, stu.age, stu.score);
    
    String s = "Hello";
    printf("字符串：%s\n", s);
    
    return 0;
}