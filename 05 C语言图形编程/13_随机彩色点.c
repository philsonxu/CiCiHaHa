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
    for(i = 0; i < 5000; i++) {
        putpixel(random(640), random(480), random(16));
    }

    // getch();
    closegraph();
    return 0;
}