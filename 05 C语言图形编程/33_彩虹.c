#include "graphics.h"
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <dos.h>

int main()
{
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");

    int i, colors[] = {RED, LIGHTRED, YELLOW, GREEN, CYAN, BLUE, MAGENTA};
    setbkcolor(LIGHTBLUE);
    //cleardevice();
    setfillstyle(SOLID_FILL, GREEN);
    bar(0, 350, 640, 480);
    for(i = 0; i < 7; i++) {
        setcolor(colors[i]);
        setfillstyle(SOLID_FILL, colors[i]);
        arc(320, 400, 0, 180, 250 - i * 15);
    }
    setfillstyle(SOLID_FILL, YELLOW);
    circle(100, 80, 30);
    floodfill(100, 80, YELLOW);

    // getch();
    closegraph();
    return 0;
}