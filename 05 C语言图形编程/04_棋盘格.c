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
    for(i = 0; i < 8; i++) {
        for(j = 0; j < 8; j++) {
            if((i + j) % 2 == 0)
                setfillstyle(SOLID_FILL, WHITE);
            else
                setfillstyle(SOLID_FILL, BLACK);
            bar(80 + i * 60, 40 + j * 60, 140 + i * 60, 100 + j * 60);
        }
    }

    // getch();
    closegraph();
    return 0;
}