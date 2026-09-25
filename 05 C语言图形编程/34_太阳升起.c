#include "graphics.h"
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <dos.h>

int main()
{
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");

    int i, y;
    for(y = 450; y > 80; y -= 2) {
        //cleardevice();
        setfillstyle(SOLID_FILL, LIGHTBLUE);
        bar(0, 0, 640, 350);
        setfillstyle(SOLID_FILL, GREEN);
        bar(0, 350, 640, 480);
        setfillstyle(SOLID_FILL, YELLOW);
        circle(320, y, 40);
        floodfill(320, y, YELLOW);
        // delay(50);
    }

    // getch();
    closegraph();
    return 0;
}