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
    setcolor(WHITE);
    circle(320, 240, 150);
    setfillstyle(SOLID_FILL, WHITE);
    pieslice(320, 240, 90, 270, 150);
    circle(320, 165, 75);
    setfillstyle(SOLID_FILL, WHITE);
    floodfill(320, 165, WHITE);
    circle(320, 315, 75);
    setfillstyle(SOLID_FILL, BLACK);
    floodfill(320, 315, WHITE);
    setfillstyle(SOLID_FILL, BLACK);
    floodfill(250, 240, WHITE);
    circle(320, 165, 20);
    setfillstyle(SOLID_FILL, BLACK);
    floodfill(320, 165, WHITE);
    circle(320, 315, 20);
    setfillstyle(SOLID_FILL, WHITE);
    floodfill(320, 315, WHITE);

    // getch();
    closegraph();
    return 0;
}