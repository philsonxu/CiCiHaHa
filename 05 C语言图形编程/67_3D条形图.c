#include "graphics.h"
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <dos.h>

int main()
{
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");

    int i, heights[] = {120, 180, 90, 150, 200, 130, 170};
    int colors[] = {RED, GREEN, BLUE, YELLOW, MAGENTA, CYAN, WHITE};
    setbkcolor(0);
    //cleardevice();
    for(i = 0; i < 7; i++) {
        int x = 80 + i * 75;
        int h = heights[i];
        setfillstyle(SOLID_FILL, colors[i]);
        bar3d(x, 400 - h, x + 50, 400, 15, 1);
    }

    // getch();
    closegraph();
    return 0;
}