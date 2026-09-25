#include "graphics.h"
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <dos.h>

int main()
{
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");

    setbkcolor(GREEN);
    //cleardevice();
    setfillstyle(SOLID_FILL, DARKGRAY);
    bar(200, 300, 440, 380);
    circle(230, 380, 20); circle(290, 380, 20);
    circle(350, 380, 20); circle(410, 380, 20);
    setfillstyle(SOLID_FILL, LIGHTGRAY);
    circle(320, 320, 50);
    floodfill(320, 320, LIGHTGRAY);
    bar(320, 310, 450, 330);

    // getch();
    closegraph();
    return 0;
}