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
    int i, a = 180;
    setbkcolor(0);
    //cleardevice();
    setcolor(LIGHTMAGENTA);
    for(i = 0; i <= 3600; i++) {
        t = i * 3.14159 / 1800;
        x = 320 + a * pow(cos(t), 3);
        y = 240 + a * pow(sin(t), 3);
        putpixel((int)x, (int)y, LIGHTMAGENTA);
    }

    // getch();
    closegraph();
    return 0;
}