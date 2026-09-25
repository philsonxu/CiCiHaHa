#include <stdio.h>
#include "graphics.h"

/*
* C 语言经典100例-57
* https://www.runoob.com/cprogramming/c-exercise-example56.html
* 题目：画圆形circle
*/
int main()
{
    int driver, mode, i;
    float j = 1, k = 1;
    driver = VGA;
    mode = VGAHI;
    initgraph(&driver, &mode, "");
    setbkcolor(YELLOW);
    for (i = 0; i <= 25; i++)
    {
        setcolor(8);
        circle(310, 250, k);
        k = k + j;
        j = j + 0.3;
    }
    closegraph();
    return 0;
}
