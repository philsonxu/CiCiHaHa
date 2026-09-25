#include "graphics.h"
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <dos.h>

void sierpinski(int x1, int y1, int x2, int y2, int x3, int y3, int n)
{
    if (n == 0) {
        line(x1, y1, x2, y2);
        line(x2, y2, x3, y3);
        line(x3, y3, x1, y1);
        return;
    }
    int mx1 = (x1 + x2) / 2, my1 = (y1 + y2) / 2;
    int mx2 = (x2 + x3) / 2, my2 = (y2 + y3) / 2;
    int mx3 = (x3 + x1) / 2, my3 = (y3 + y1) / 2;
    sierpinski(x1, y1, mx1, my1, mx3, my3, n - 1);
    sierpinski(mx1, my1, x2, y2, mx2, my2, n - 1);
    sierpinski(mx3, my3, mx2, my2, x3, y3, n - 1);
}

int main()
{
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");

    setbkcolor(0);
    //cleardevice();
    setcolor(LIGHTCYAN);
    sierpinski(100, 400, 540, 400, 320, 80, 6);

    // getch();
    closegraph();
    return 0;
}