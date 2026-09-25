#include "graphics.h"
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <dos.h>

int main()
{
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");

    int x;
    double y;
    setbkcolor(0);
    //cleardevice();
    setcolor(WHITE);
    line(50, 240, 590, 240);
    line(320, 50, 320, 430);
    setcolor(LIGHTGREEN);
    for(x = -200; x < 200; x++) {
        y = 240 - x * x / 200.0;
        putpixel(320 + x, (int)y, LIGHTGREEN);
    }

    // getch();
    closegraph();
    return 0;
}