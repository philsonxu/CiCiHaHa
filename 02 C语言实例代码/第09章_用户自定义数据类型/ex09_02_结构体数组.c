/*
 * 例9.2 结构体数组
 * 知识点：多个结构体组成数组，批量处理同类数据
 */
#include <stdio.h>

struct Student {
    int num;
    char name[20];
    float score;
};

int main()
{
    struct Student stus[3] = {
        {1001, "Zhang", 85.5},
        {1002, "Li",    92.0},
        {1003, "Wang",  78.5}
    };
    
    int i;
    float sum = 0;
    printf("学生信息：\n");
    for (i = 0; i < 3; i++) {
        printf("学号：%d, 姓名：%s, 成绩：%.1f\n",
               stus[i].num, stus[i].name, stus[i].score);
        sum += stus[i].score;
    }
    printf("平均成绩：%.2f\n", sum / 3);
    
    /* 找成绩最高的学生 */
    int max_idx = 0;
    for (i = 1; i < 3; i++) {
        if (stus[i].score > stus[max_idx].score)
            max_idx = i;
    }
    printf("最高分学生：%s, 成绩：%.1f\n", 
           stus[max_idx].name, stus[max_idx].score);
    
    return 0;
}