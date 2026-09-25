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
    for(i = 0; i < 8; i++) {
        setcolor(i + 1);
        setfillstyle(SOLID_FILL, i + 1);
        sector(320, 240, i * 45, i * 45 + 45, 150, 100);
    }

    // getch();
    closegraph();
    return 0;
}