/*
 * 例11.3 文件包含 #include
 * 知识点：
 *   #include <文件名>  系统头文件，从系统目录查找
 *   #include "文件名"  自定义头文件，先从当前目录查找
 * 预处理时将被包含文件内容原封不动插入到该位置
 *
 * 本例展示自定义头文件的使用方法（头文件通常放函数声明、宏定义等）
 */
#include <stdio.h>
#include <math.h>      /* 标准库头文件 */

/* ========== 通常放在 .h 头文件中 ========== */
/* mymath.h 内容 */
#ifndef MYMATH_H
#define MYMATH_H
#define MY_PI 3.1415926
double circle_area(double r);
double circle_perimeter(double r);
#endif
/* ========================================== */

/* ========== 通常放在 .c 源文件中 ========== */
/* mymath.c 内容 */
double circle_area(double r)
{
    return MY_PI * r * r;
}

double circle_perimeter(double r)
{
    return 2 * MY_PI * r;
}
/* ========================================== */

int main()
{
    double r = 5.0;
    printf("半径%.1f的圆：\n", r);
    printf("面积 = %.2f\n", circle_area(r));
    printf("周长 = %.2f\n", circle_perimeter(r));
    return 0;
}