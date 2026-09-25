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
    bar(0, 400, 640, 480);
    setfillstyle(SOLID_FILL, LIGHTGRAY);
    line(320, 150, 320, 350);
    line(250, 280, 390, 280);
    line(320, 150, 250, 280);
    line(320, 150, 390, 280);
    line(250, 280, 320, 350);
    line(390, 280, 320, 350);
    floodfill(320, 250, LIGHTGRAY);
    setfillstyle(SOLID_FILL, DARKGRAY);
    line(200, 280, 250, 270);
    line(200, 290, 250, 290);
    line(200, 280, 200, 290);
    floodfill(220, 285, DARKGRAY);
    line(440, 280, 390, 270);
    line(440, 290, 390, 290);
    line(440, 280, 440, 290);
    floodfill(420, 285, DARKGRAY);
    setfillstyle(SOLID_FILL, CYAN);
    circle(330, 200, 15);
    floodfill(330, 200, CYAN);

    // getch();
    closegraph();
    return 0;
}