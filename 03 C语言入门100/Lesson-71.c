#include <stdio.h>
#include <stdlib.h>

/*
* C 语言实例 - 结构体二进制文件读写
* https://www.runoob.com/cprogramming/c-examples.html
*/

// 定义学生结构体
// 注意：必须使用固定长度数组，不能用 char *name 指针成员
typedef struct {
    int id;             // 学号
    char name[50];      // 姓名（固定数组，不可用指针）
    float score;        // 成绩
} Student;

int main()
{
    // ===== 写入结构体到文件 =====
    Student s = { 101, "Alice", 95.5f };  // 初始化数据
    FILE* fp = fopen("student.bin", "wb");
    if (fp == NULL) {
        perror("文件打开失败");
        return 1;
    }
    // 将整个结构体作为一块数据写入
    size_t written = fwrite(&s, sizeof(Student), 1, fp);
    if (written != 1) {
        printf("写入失败\n");
    }
    fclose(fp);

    // ===== 从文件读取结构体 =====
    fp = fopen("student.bin", "rb");
    if (fp == NULL) {
        perror("文件打开失败");
        return 1;
    }
    Student loaded;  // 未初始化，将由 fread 填充
    size_t read_count = fread(&loaded, sizeof(Student), 1, fp);
    if (read_count != 1) {
        printf("读取失败或文件结束\n");
        fclose(fp);
        return 1;
    }
    printf("学号: %d, 姓名: %s, 成绩: %.1f\n", loaded.id, loaded.name, loaded.score);
    fclose(fp);
    return 0;
}