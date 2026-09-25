#include <stdio.h>

/*
* C 语言实例 - 二进制文件定位
* https://www.runoob.com/cprogramming/c-examples.html
*/

typedef struct {
    int id;
    char name[50];
    float score;
} Student;

int main()
{
    FILE* fp = fopen("students.bin", "rb");
    if (fp == NULL) {
        perror("文件打开失败");
        return 1;
    }

    // 跳到文件末尾，获取文件大小
    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    printf("文件大小: %ld 字节\n", file_size);

    // 计算文件中有多少个结构体记录
    long record_count = file_size / sizeof(Student);
    printf("包含 %ld 条学生记录\n", record_count);

    // 跳到第 6 个结构体（索引从 0 开始，偏移量 = 5 * sizeof(Student)）
    fseek(fp, sizeof(Student) * 5, SEEK_SET);
    Student s;
    fread(&s, sizeof(Student), 1, fp);
    printf("第 6 条记录: id=%d, name=%s\n", s.id, s.name);

    fclose(fp);
    return 0;
}