#include "stdio.h"
#include "graphics.h"

/*
* C 语言经典100例-64
* https://www.runoob.com/cprogramming/c-exercise-example64.html
* 题目：画椭圆与矩形ellipse and rectangle
*/
int main()
{
    int driver = VGA, mode = VGAHI;
    int i, num = 15, top = 50;
    int left = 20, right = 50;
    initgraph(&driver, &mode, "");
    for (i = 0; i < num; i++)
    {
        ellipse(250, 250, 0, 360, right, left);
        ellipse(250, 250, 0, 360, 20, top);
        rectangle(20 - 2 * i, 20 - 2 * i, 10 * (i + 2), 10 * (i + 2));
        right += 5;
        left += 5;
        top += 10;
    }
    closegraph();
    return 0;
}