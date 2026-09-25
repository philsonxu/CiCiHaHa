#include "graphics.h"
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <dos.h>

int main()
{
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");

    int i, j;
    setbkcolor(0);
    //cleardevice();
    for(i = 0; i < 16; i++) {
        for(j = 0; j < 12; j++) {
            setfillstyle(SOLID_FILL, (i * j) % 16);
            bar(10 + i * 40, 10 + j * 40, 40 + i * 40, 40 + j * 40);
        }
    }

    // getch();
    closegraph();
    return 0;
}