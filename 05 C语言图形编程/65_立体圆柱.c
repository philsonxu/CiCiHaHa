#include "graphics.h"
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <dos.h>

int main()
{
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");

    int i;
    setbkcolor(0);
    //cleardevice();
    setcolor(LIGHTGRAY);
    ellipse(320, 150, 0, 360, 100, 30);
    ellipse(320, 350, 0, 360, 100, 30);
    line(220, 150, 220, 350);
    line(420, 150, 420, 350);
    setfillstyle(SOLID_FILL, LIGHTGRAY);
    ellipse(320, 350, 0, 180, 100, 30);
    floodfill(320, 370, LIGHTGRAY);
    setfillstyle(SOLID_FILL, DARKGRAY);
    floodfill(320, 250, LIGHTGRAY);
    setfillstyle(SOLID_FILL, LIGHTGRAY);
    floodfill(320, 130, LIGHTGRAY);

    // getch();
    closegraph();
    return 0;
}