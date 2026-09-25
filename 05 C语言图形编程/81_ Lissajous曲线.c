#include "graphics.h"
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <dos.h>

int main()
{
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");

    double t, x, y;
    int i;
    int a = 3, b = 2;
    double delta = 3.14159 / 2;
    setbkcolor(0);
    //cleardevice();
    setcolor(YELLOW);
    for(i = 0; i < 10000; i++) {
        t = i * 0.001;
        x = 320 + 180 * sin(a * t + delta);
        y = 240 + 180 * sin(b * t);
        putpixel((int)x, (int)y, YELLOW);
    }

    // getch();
    closegraph();
    return 0;
}