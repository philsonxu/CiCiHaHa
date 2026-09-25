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
    setbkcolor(BLACK);
    //cleardevice();
    for(i = 1; i <= 15; i++) {
        setcolor(i);
        setfillstyle(SOLID_FILL, i);
        circle(320, 240, 200 - i * 12);
        floodfill(320, 240, i);
    }

    // getch();
    closegraph();
    return 0;
}