#include "graphics.h"
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <dos.h>

int main()
{
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");

    double x, y;
    setbkcolor(0);
    //cleardevice();
    setcolor(WHITE);
    line(50, 240, 590, 240);
    line(320, 50, 320, 430);
    setcolor(LIGHTBLUE);
    for(y = -200; y < 200; y += 0.1) {
        x = sqrt(10000 + y * y / 4);
        putpixel(320 + (int)x, 240 + (int)y, LIGHTBLUE);
        putpixel(320 - (int)x, 240 + (int)y, LIGHTBLUE);
    }

    // getch();
    closegraph();
    return 0;
}