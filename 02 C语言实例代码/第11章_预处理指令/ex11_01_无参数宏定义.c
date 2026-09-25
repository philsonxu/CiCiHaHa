/*
 * 例11.1 无参数宏定义 #define
 * 知识点：宏定义在预处理阶段进行简单文本替换，不做语法检查
 * 宏名习惯用大写字母（区分变量），末尾不加分号
 */
#include <stdio.h>

#define PI 3.1415926
#define MAX_SIZE 100
#define HELLO "Hello World"
#define NEWLINE printf("\n")

int main()
{
    float r = 5.0f;
    float area = PI * r * r;
    printf("半径%.1f的圆面积 = %.2f\n", r, area);
    
    int arr[MAX_SIZE];
    printf("数组大小：%d\n", MAX_SIZE);
    
    printf("输出字符串：%s", HELLO);
    NEWLINE;
    
    /* 宏可以嵌套使用 */
#define R 2.0f
#define AREA PI * R * R
    printf("AREA = %.2f\n", AREA);
    
    return 0;
}