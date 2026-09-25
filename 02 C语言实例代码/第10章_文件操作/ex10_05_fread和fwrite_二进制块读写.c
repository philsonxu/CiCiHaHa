/*
 * 例10.5 fread/fwrite 二进制块读写
 * 知识点：以二进制方式读写整块数据（结构体、数组等），效率高
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
    
    /* 二进制写入 */
    fp = fopen("student.dat", "wb");
    if (fp == NULL) {
        printf("打开文件失败！\n");
        exit(1);
    }
    
    fwrite(stu, sizeof(struct Student), 3, fp);  /* 一次写3个学生 */
    fclose(fp);
    printf("二进制数据已写入student.dat\n");
    
    /* 二进制读取 */
    printf("\n从二进制文件读取：\n");
    fp = fopen("student.dat", "rb");
    for (i = 0; i < 3; i++) {
        fread(&s, sizeof(struct Student), 1, fp);
        printf("学号：%d, 姓名：%s, 成绩：%.1f\n", s.num, s.name, s.score);
    }
    fclose(fp);
    
    return 0;
}