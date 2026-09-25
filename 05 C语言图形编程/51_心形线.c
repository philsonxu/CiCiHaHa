#include "graphics.h"
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <dos.h>

int main()
{
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");

    double t, r, x, y;
    setbkcolor(0);
    //cleardevice();
    setcolor(RED);
    for(t = 0; t < 2 * 3.14159; t += 0.001) {
        r = 150 * (1 - cos(t));
        x = 320 + r * cos(t);
        y = 240 + r * sin(t);
        putpixel((int)x, (int)y, RED);
    }

    // getch();
    closegraph();
    return 0;
}