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
    setbkcolor(BLACK);
    //cleardevice();
    setfillstyle(SOLID_FILL, BROWN);
    bar(300, 380, 340, 450);
    setfillstyle(SOLID_FILL, GREEN);
    for(i = 0; i < 4; i++) {
        int y = 200 + i * 50;
        int w = 60 + i * 40;
        line(320, y - 40, 320 - w, y);
        line(320, y - 40, 320 + w, y);
        line(320 - w, y, 320 + w, y);
        floodfill(320, y - 10, GREEN);
    }
    for(i = 0; i < 30; i++) {
        setfillstyle(SOLID_FILL, random(15) + 1);
        circle(200 + random(240), 200 + random(180), 3);
        floodfill(200 + random(240), 200 + random(180), 1);
    }
    setfillstyle(SOLID_FILL, YELLOW);
    circle(320, 150, 15);
    floodfill(320, 150, YELLOW);

    // getch();
    closegraph();
    return 0;
}