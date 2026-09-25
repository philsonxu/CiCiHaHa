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
    double angle, x[5], y[5];
    setbkcolor(0);
    //cleardevice();
    for(i = 0; i < 5; i++) {
        angle = (i * 72 - 90) * 3.14159 / 180;
        x[i] = 320 + 150 * cos(angle);
        y[i] = 240 + 150 * sin(angle);
    }
    setcolor(YELLOW);
    for(i = 0; i < 5; i++) {
        line((int)x[i], (int)y[i], (int)x[(i+2)%5], (int)y[(i+2)%5]);
    }
    setfillstyle(SOLID_FILL, YELLOW);
    floodfill(320, 240, YELLOW);

    // getch();
    closegraph();
    return 0;
}