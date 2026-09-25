#include "graphics.h"
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <dos.h>

int main()
{
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");

    int x;
    double y;
    setbkcolor(0);
    //cleardevice();
    setcolor(WHITE);
    line(50, 240, 590, 240);
    line(320, 50, 320, 430);
    setcolor(CYAN);
    for(x = 0; x < 540; x++) {
        y = 240 - 150 * cos((x - 50) * 4 * 3.14159 / 540);
        putpixel(50 + x, (int)y, CYAN);
    }

    // getch();
    closegraph();
    return 0;
}