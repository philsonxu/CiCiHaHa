#include "graphics.h"
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <dos.h>

int main()
{
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");

    double t, x, y, z;
    int i;
    setbkcolor(0);
    //cleardevice();
    for(i = 0; i < 5000; i++) {
        t = i * 0.02;
        x = 320 + (100 + 30 * sin(t * 5)) * cos(t);
        y = 240 + (100 + 30 * sin(t * 5)) * sin(t);
        putpixel((int)x, (int)y, i % 16);
    }

    // getch();
    closegraph();
    return 0;
}