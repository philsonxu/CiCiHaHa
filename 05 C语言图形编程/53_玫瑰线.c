#include "graphics.h"
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <dos.h>

int main()
{
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");

    double theta, r, x, y;
    int i, k = 5;
    setbkcolor(0);
    //cleardevice();
    setcolor(MAGENTA);
    for(i = 0; i < 3600; i++) {
        theta = i * 3.14159 / 1800;
        r = 180 * cos(k * theta);
        x = 320 + r * cos(theta);
        y = 240 + r * sin(theta);
        putpixel((int)x, (int)y, MAGENTA);
    }

    // getch();
    closegraph();
    return 0;
}