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
    setfillstyle(SOLID_FILL, DARKGRAY);
    int peaks[] = {0, 200, 100, 280, 150, 250, 250, 320, 350, 230,
                   420, 300, 500, 200, 580, 280, 640, 220, 640, 480, 0, 480};
    for(i = 0; i < 10; i++) {
        line(peaks[i*2], peaks[i*2+1], peaks[i*2+2], peaks[i*2+3]);
    }
    floodfill(320, 350, DARKGRAY);
    setfillstyle(SOLID_FILL, GREEN);
    bar(0, 380, 640, 480);
    setfillstyle(SOLID_FILL, WHITE);
    circle(320, 100, 25);
    floodfill(320, 100, WHITE);
    circle(345, 90, 30);
    floodfill(345, 90, WHITE);
    circle(360, 110, 25);
    floodfill(360, 110, WHITE);

    // getch();
    closegraph();
    return 0;
}