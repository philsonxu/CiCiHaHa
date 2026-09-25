/*
 * 例4.6 switch语句 - 成绩等级判断
 * 知识点：switch-case结构、break穿透特性、多分支选择
 */
#include <stdio.h>

int main()
{
    int score;
    char grade;
    printf("请输入成绩（0-100）：");
    scanf("%d", &score);
    
    if (score < 0 || score > 100) {
        printf("输入的成绩不合法！\n");
        return 0;
    }
    
    switch (score / 10) {
        case 10:
        case 9:  grade = 'A'; break;
        case 8:  grade = 'B'; break;
        case 7:  grade = 'C'; break;
        case 6:  grade = 'D'; break;
        default: grade = 'E';
    }
    
    printf("成绩等级为：%c\n", grade);
    return 0;
}