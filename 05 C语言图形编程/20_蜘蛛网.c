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
    setcolor(WHITE);
    for(i = 1; i <= 8; i++) {
        ellipse(320, 240, 0, 360, i * 20, i * 15);
    }
    for(j = 0; j < 12; j++) {
        double angle = j * 30 * 3.14159 / 180;
        line(320, 240, 320 + 160 * cos(angle), 240 + 120 * sin(angle));
    }

    // getch();
    closegraph();
    return 0;
}