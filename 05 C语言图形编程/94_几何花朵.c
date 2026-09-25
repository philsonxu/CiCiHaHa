#include "graphics.h"
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <dos.h>

int main()
{
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");

    int i, j;
    setbkcolor(0);
    //cleardevice();
    for(i = 0; i < 12; i++) {
        double angle = i * 30 * 3.14159 / 180;
        setcolor(MAGENTA);
        setfillstyle(SOLID_FILL, MAGENTA);
        ellipse(320 + 50 * cos(angle), 240 + 50 * sin(angle),
                0, 360, 40, 20);
        floodfill(320 + 50 * cos(angle), 240 + 50 * sin(angle), MAGENTA);
    }
    setcolor(YELLOW);
    setfillstyle(SOLID_FILL, YELLOW);
    circle(320, 240, 30);
    floodfill(320, 240, YELLOW);
    setcolor(GREEN);
    setfillstyle(SOLID_FILL, GREEN);
    bar(315, 270, 325, 450);

    // getch();
    closegraph();
    return 0;
}