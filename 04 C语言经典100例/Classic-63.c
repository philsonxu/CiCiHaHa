#include "stdio.h"
#include "graphics.h"

/*
* C 语言经典100例-63
* https://www.runoob.com/cprogramming/c-exercise-example63.html
* 题目：画椭圆ellipse
*/
int main()
{
    int x = 360, y = 160, driver = VGA, mode = VGAHI;
    int num = 20, i;
    int top, bottom;
    initgraph(&driver, &mode, "");
    top = y - 30;
    bottom = y - 30;
    for (i = 0; i < num; i++)
    {
        ellipse(250, 250, 0, 360, top, bottom);
        top -= 5;
        bottom += 5;
    }

    closegraph();
    return 0;
}