#include "graphics.h"
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <dos.h>

int main()
{
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");

    int i, size = 20;
    setbkcolor(0);
    //cleardevice();
    for(i = 0; i < 12; i++) {
        setcolor(i % 16);
        int x = 320, y = 40 + i * 35;
        line(x, y, x + size, y + size);
        line(x + size, y + size, x, y + 2 * size);
        line(x, y + 2 * size, x - size, y + size);
        line(x - size, y + size, x, y);
        size += 3;
    }

    // getch();
    closegraph();
    return 0;
}