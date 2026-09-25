#include "stdio.h"
#include "graphics.h"

/*
* C 语言经典100例-62
* https://www.runoob.com/cprogramming/c-exercise-example62.html
* 题目：画点putpixel
*/
int main()
{
    int i, j, driver = VGA, mode = VGAHI;
    initgraph(&driver, &mode, "");
    setbkcolor(YELLOW);
    for (i = 50; i <= 230; i += 20)
        for (j = 50; j <= 230; j++)
            putpixel(i, j, 1);
    for (j = 50; j <= 230; j += 20)
        for (i = 50; i <= 230; i++)
            putpixel(i, j, 1);

    closegraph();
    return 0;
}