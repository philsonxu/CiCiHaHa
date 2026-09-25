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
    for(i = 0; i < 640; i += 20) {
        setcolor(i / 20 % 16);
        line(i, 0, i, 480);
    }
    for(i = 0; i < 480; i += 20) {
        setcolor(i / 20 % 16);
        line(0, i, 640, i);
    }

    // getch();
    closegraph();
    return 0;
}