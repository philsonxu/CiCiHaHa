#include "graphics.h"
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <dos.h>

void cross(int x, int y, int size, int n)
{
    if (n == 0) return;
    setcolor(n % 16);
    setfillstyle(SOLID_FILL, n % 16);
    bar(x - size / 2, y - size / 6, x + size / 2, y + size / 6);
    bar(x - size / 6, y - size / 2, x + size / 6, y + size / 2);
    cross(x - size / 2, y, size / 2, n - 1);
    cross(x + size / 2, y, size / 2, n - 1);
    cross(x, y - size / 2, size / 2, n - 1);
    cross(x, y + size / 2, size / 2, n - 1);
}

int main()
{
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");

    setbkcolor(0);
    //cleardevice();
    cross(320, 240, 200, 5);

    // getch();
    closegraph();
    return 0;
}