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
    for(i = 0; i < 20; i++) {
        for(j = 0; j < 20; j++) {
            int c = (i + j) % 14 + 1;
            setfillstyle(SOLID_FILL, c);
            if((i / 2 + j / 2) % 2 == 0)
                bar(j * 32, i * 24, j * 32 + 28, i * 24 + 20);
            else
                bar(j * 32 + 4, i * 24 + 4, j * 32 + 32, i * 24 + 24);
        }
    }

    // getch();
    closegraph();
    return 0;
}