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
    double theta, r;
    setbkcolor(0);
    //cleardevice();
    for(i = 0; i < 720; i++) {
        theta = i * 3.14159 / 180;
        r = i / 3.0;
        setcolor(i % 16);
        putpixel(320 + r * cos(theta), 240 + r * sin(theta), i % 16);
    }

    // getch();
    closegraph();
    return 0;
}