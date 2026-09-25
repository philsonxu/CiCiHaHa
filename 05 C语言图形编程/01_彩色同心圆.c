#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <dos.h>
#include "graphics.h"

int main()
{
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");

    int i;
    setbkcolor(0);
    //cleardevice();
    for(i = 0; i <= 15; i++) {
        setcolor(i);
        circle(320, 240, 20 + i * 12);
        // delay(50);
    }

    // getch();
    closegraph();
    return 0;
}