#include "graphics.h"
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <dos.h>

void tree(int x, int y, double angle, int len, int n)
{
    int x2 = x + len * cos(angle);
    int y2 = y - len * sin(angle);
    setcolor(n % 16);
    line(x, y, x2, y2);
    if (n > 0) {
        tree(x2, y2, angle - 0.4, len * 0.7, n - 1);
        tree(x2, y2, angle + 0.4, len * 0.7, n - 1);
    }
}

int main()
{
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");

    setbkcolor(0);
    //cleardevice();
    tree(320, 450, 3.14159 / 2, 100, 9);

    // getch();
    closegraph();
    return 0;
}