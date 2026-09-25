#include <stdio.h>
/*
* C 语言实例 - 使用结构体（struct）
* https://www.runoob.com/cprogramming/c-examples.html
*/
struct student
{
    char name[50];
    int roll;
    float marks;
} s;

int main()
{
    printf("输入信息:\n");

    printf("名字: ");
    scanf("%s", s.name);

    printf("编号: ");
    scanf("%d", &s.roll);

    printf("成绩: ");
    scanf("%f", &s.marks);


    printf("显示信息:\n");

    printf("名字: ");
    puts(s.name);

    printf("编号: %d\n", s.roll);

    printf("成绩: %.1f\n", s.marks);

    return 0;
}

struct
{
  int i;
  char j;
} s1;

#pragma pack(1)
struct
{
  int i;
  char j;
} s2;

int main(void)
{
    int size_s1;
    int size_s2;

    size_s1 = sizeof(s1);
    size_s2 = sizeof(s2);

    printf(" 默认对齐方式下size_s1 = %d\n", size_s1);
    printf(" 按字节对齐方式下size_s2 = %d\n", size_s2);
}
