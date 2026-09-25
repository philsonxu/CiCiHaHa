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
    setbkcolor(LIGHTBLUE);
    //cleardevice();
    setfillstyle(SOLID_FILL, WHITE);
    for(i = 0; i < 5; i++) {
        int x = 50 + i * 120;
        int y = 80 + random(80);
        circle(x, y, 25);
        floodfill(x, y, WHITE);
        circle(x + 30, y + 5, 30);
        floodfill(x + 30, y + 5, WHITE);
        circle(x + 60, y, 25);
        floodfill(x + 60, y, WHITE);
    }
    setfillstyle(SOLID_FILL, GREEN);
    bar(0, 400, 640, 480);
    setfillstyle(SOLID_FILL, YELLOW);
    circle(550, 80, 35);
    floodfill(550, 80, YELLOW);

    // getch();
    closegraph();
    return 0;
}