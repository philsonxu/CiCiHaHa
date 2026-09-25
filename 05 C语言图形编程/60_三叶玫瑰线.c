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
    int i;
    setbkcolor(0);
    //cleardevice();
    setcolor(BROWN);
    for(i = 0; i < 3600; i++) {
        theta = i * 3.14159 / 1800;
        r = 180 * sin(3 * theta);
        x = 320 + r * cos(theta);
        y = 240 + r * sin(theta);
        putpixel((int)x, (int)y, BROWN);
    }

    // getch();
    closegraph();
    return 0;
}