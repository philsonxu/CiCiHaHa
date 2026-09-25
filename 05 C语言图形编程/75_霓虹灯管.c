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
    for(i = 0; i < 15; i++) {
        setcolor(i);
        circle(320, 240, 50 + i * 10);
    }
    for(i = 0; i < 15; i++) {
        setcolor(i);
        line(100, 100 + i * 20, 540, 100 + i * 20);
    }

    // getch();
    closegraph();
    return 0;
}