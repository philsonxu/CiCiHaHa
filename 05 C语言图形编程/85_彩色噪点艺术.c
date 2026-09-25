#include "graphics.h"
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <dos.h>

int main()
{
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");

    int i, x, y;
    setbkcolor(0);
    //cleardevice();
    for(i = 0; i < 10000; i++) {
        x = random(640);
        y = random(480);
        int c = (x / 40 + y / 30) % 16;
        putpixel(x, y, c);
    }

    // getch();
    closegraph();
    return 0;
}