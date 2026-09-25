#include "graphics.h"
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <dos.h>

int main()
{
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");

    double theta, x, y;
    int i, R = 50, r = 20;
    setbkcolor(0);
    //cleardevice();
    setcolor(YELLOW);
    for(i = 0; i < 2000; i++) {
        theta = i * 0.01;
        x = 100 + (R - r) * theta + r * sin((R - r) * theta / r);
        y = 240 + (R - r) * cos((R - r) * theta / r);
        if(x >= 0 && x < 640) putpixel((int)x, (int)y, YELLOW);
    }

    // getch();
    closegraph();
    return 0;
}