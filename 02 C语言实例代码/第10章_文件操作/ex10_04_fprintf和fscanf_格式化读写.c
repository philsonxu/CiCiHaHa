/*
 * 例10.4 fprintf/fscanf 格式化读写
 * 知识点：类似printf/scanf，但面向文件
 */
#include <stdio.h>
#include <stdlib.h>

struct Student {
    int num;
    char name[20];
    float score;
};

int main()
{
    FILE *fp;
    struct Student stu[3] = {
        {1001, "Zhang", 85.5},
        {1002, "Li",    92.0},
        {1003, "Wang",  78.5}
    };
    struct Student s;
    int i;
    
    /* 写入格式化数据 */
    fp = fopen("student.txt", "w");
    if (fp == NULL) {
        printf("打开文件失败！\n");
        exit(1);
    }
    
    for (i = 0; i < 3; i++) {
        fprintf(fp, "%d %s %.1f\n", stu[i].num, stu[i].name, stu[i].score);
    }
    fclose(fp);
    printf("数据已写入student.txt\n");
    
    /* 读取格式化数据 */
    printf("\n从文件读取：\n");
    fp = fopen("student.txt", "r");
    while (fscanf(fp, "%d %s %f", &s.num, s.name, &s.score) != EOF) {
        printf("学号：%d, 姓名：%s, 成绩：%.1f\n", s.num, s.name, s.score);
    }
    fclose(fp);
    
    return 0;
}