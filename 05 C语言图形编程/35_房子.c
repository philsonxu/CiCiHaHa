#include "graphics.h"
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <dos.h>

int main()
{
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");

    setbkcolor(LIGHTBLUE);
    //cleardevice();
    setfillstyle(SOLID_FILL, GREEN);
    bar(0, 350, 640, 480);
    setfillstyle(SOLID_FILL, BROWN);
    bar(220, 250, 420, 400);
    setfillstyle(SOLID_FILL, RED);
    line(220, 250, 320, 150);
    line(420, 250, 320, 150);
    line(220, 250, 420, 250);
    floodfill(320, 200, RED);
    setfillstyle(SOLID_FILL, BLUE);
    bar(260, 290, 310, 340);
    bar(330, 290, 380, 340);
    setfillstyle(SOLID_FILL, BROWN);
    bar(310, 340, 330, 400);
    setfillstyle(SOLID_FILL, YELLOW);
    circle(500, 100, 30);
    floodfill(500, 100, YELLOW);

    // getch();
    closegraph();
    return 0;
}