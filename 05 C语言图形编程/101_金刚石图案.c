#include <stdio.h>
#include <math.h>
#include "graphics.h"

/*
* 使用turbo C 绘制金刚石图案
* https://blog.csdn.net/LCYong_/article/details/53313350
*/
int main()
{
    int gdriver = DETECT, gmode;
    int r, n, x0 = 230, y0 = 230;
    double arc;
    int i, j;
    double x[100], y[100];
    initgraph(&gdriver, &gmode, "");
    printf("please input r and n:");
    scanf("%d%d", &r, &n);
    arc = 2 * 3.14159 / n;
    for (i = 0; i < n; i++)
    {
        x[i] = x0 + r * cos(i * arc);
        y[i] = y0 + r * sin(i * arc);
    }
    setcolor(4);
    setbkcolor(0);
    for (i = 0; i < n - 1; i++)
        for (j = i + 1; j <= n - 1; j++)
            line(x[i], y[i], x[j], y[j]);
    //getchar();
    closegraph();
}