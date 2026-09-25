#include "graphics.h"
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <dos.h>

void dragon(int x1, int y1, int x2, int y2, int n, int sign) 
{
    if (n == 0) {
        line(x1, y1, x2, y2);
        return;
    }
    int mx = (x1 + x2) / 2 + sign * (y2 - y1) / 2;
    int my = (y1 + y2) / 2 - sign * (x2 - x1) / 2;
    dragon(x1, y1, mx, my, n - 1, 1);
    dragon(x2, y2, mx, my, n - 1, -1);
}

int main()
{
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");

    setbkcolor(0);
    //cleardevice();
    setcolor(LIGHTMAGENTA);
    dragon(200, 200, 440, 200, 12, 1);

    // getch();
    closegraph();
    return 0;
}