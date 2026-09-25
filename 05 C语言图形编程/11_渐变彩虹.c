#include "graphics.h"
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <dos.h>

int main()
{
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");

    int i, x;
    setbkcolor(0);
    //cleardevice();
    for(i = 0; i < 256; i++) {
        setcolor(i % 16);
        for(x = 0; x < 640; x++) {
            putpixel(x, i * 2, i % 16);
        }
    }

    // getch();
    closegraph();
    return 0;
}