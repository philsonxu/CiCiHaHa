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
                setfillstyle(SOLID_FILL, LIGHTGRAY);
            else
                setfillstyle(SOLID_FILL, DARKGRAY);
            bar(120 + i * 50, 40 + j * 50, 170 + i * 50, 90 + j * 50);
        }
    }
    setcolor(RED);
    circle(395, 65, 15);
    setfillstyle(SOLID_FILL, RED);
    floodfill(395, 65, RED);

    // getch();
    closegraph();
    return 0;
}