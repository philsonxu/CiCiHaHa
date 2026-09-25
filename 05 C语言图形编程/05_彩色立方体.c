#include "graphics.h"
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <dos.h>

int main()
{
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");

    setbkcolor(0);
    //cleardevice();
    setcolor(LIGHTRED);
    rectangle(200, 140, 400, 340);
    setcolor(LIGHTGREEN);
    line(200, 140, 250, 90);
    line(400, 140, 450, 90);
    line(250, 90, 450, 90);
    setcolor(LIGHTBLUE);
    line(400, 340, 450, 290);
    line(450, 90, 450, 290);
    setfillstyle(SOLID_FILL, LIGHTRED);
    floodfill(300, 240, LIGHTRED);
    setfillstyle(SOLID_FILL, LIGHTGREEN);
    floodfill(350, 110, LIGHTGREEN);
    setfillstyle(SOLID_FILL, LIGHTBLUE);
    floodfill(420, 200, LIGHTBLUE);

    // getch();
    closegraph();
    return 0;
}