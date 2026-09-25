/*
 * 例9.4 结构体作为函数参数
 * 知识点：值传递（传整个结构体）、地址传递（传结构体指针，效率高）
 */
#include <stdio.h>

struct Point {
    float x;
    float y;
};

/* 值传递：修改形参不影响实参 */
void move_point(struct Point p, float dx, float dy)
{
    p.x += dx;
    p.y += dy;
    printf("函数内：(%.1f, %.1f)\n", p.x, p.y);
}

/* 地址传递：通过指针修改实参 */
void move_point_ptr(struct Point *p, float dx, float dy)
{
    p->x += dx;
    p->y += dy;
}

/* 输出点 */
void print_point(struct Point p)
{
    printf("(%.1f, %.1f)\n", p.x, p.y);
}

int main()
{
    struct Point pt = {1.0f, 2.0f};
    printf("原始点：");
    print_point(pt);
    
    move_point(pt, 3, 4);
    printf("值传递后实参未变：");
    print_point(pt);
    
    move_point_ptr(&pt, 3, 4);
    printf("地址传递后实参改变：");
    print_point(pt);
    
    return 0;
}