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
    setcolor(GREEN);
    for(i = 0; i < 20; i++) {
        int y = 100 + i * 20;
        line(0, y, 640, y);
    }
    for(i = -10; i <= 10; i++) {
        line(320 + i * 30, 100, 320 + i * 200, 480);
    }

    // getch();
    closegraph();
    return 0;
}