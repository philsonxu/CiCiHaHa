#include "graphics.h"
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <dos.h>

int main()
{
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");

    setbkcolor(0);
    //cleardevice();
    setcolor(LIGHTCYAN);
    line(220, 350, 320, 120);
    line(420, 350, 320, 120);
    ellipse(320, 350, 0, 360, 100, 30);
    setfillstyle(SOLID_FILL, LIGHTCYAN);
    ellipse(320, 350, 0, 180, 100, 30);
    floodfill(320, 370, LIGHTCYAN);
    setfillstyle(SOLID_FILL, CYAN);
    floodfill(320, 250, LIGHTCYAN);

    // getch();
    closegraph();
    return 0;
}