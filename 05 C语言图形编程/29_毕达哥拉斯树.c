#include "graphics.h"
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <dos.h>

void pythagoras(int x, int y, int a, double angle, int n) 
{
    int x1 = x + a * cos(angle);
    int y1 = y - a * sin(angle);
    int x2 = x1 - a * sin(angle);
    int y2 = y1 - a * cos(angle);
    int x3 = x - a * sin(angle);
    int y3 = y - a * cos(angle);
    setcolor(n % 16);
    line(x, y, x1, y1);
    line(x1, y1, x2, y2);
    line(x2, y2, x3, y3);
    line(x3, y3, x, y);
    if (n == 0) return;
    int a2 = a * 0.707;
    pythagoras(x3, y3, a2, angle + 3.14159 / 4, n - 1);
    pythagoras(x2, y2, a2, angle - 3.14159 / 4, n - 1);
}

int main()
{
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");

    setbkcolor(0);
    //cleardevice();
    pythagoras(290, 420, 60, 3.14159/2, 10);

    // getch();
    closegraph();
    return 0;
}