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
    setbkcolor(0);
    //cleardevice();
    for(i = 0; i < 360; i += 15) {
        setcolor(i / 15 % 16);
        setfillstyle(SOLID_FILL, i / 15 % 16);
        pieslice(320, 240, i, i + 15, 150);
    }

    // getch();
    closegraph();
    return 0;
}