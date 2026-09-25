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
    double angle;
    setbkcolor(0);
    //cleardevice();
    for(i = 0; i < 12; i++) {
        angle = i * 30 * 3.14159 / 180;
        setcolor(i + 1);
        for(j = 0; j < 6; j++) {
            double r = 30 + j * 25;
            arc(320, 240, i * 30 - 15, i * 30 + 15, r);
        }
        double x = 320 + 180 * cos(angle);
        double y = 240 + 180 * sin(angle);
        circle((int)x, (int)y, 15);
    }
    setcolor(YELLOW);
    circle(320, 240, 20);

    // getch();
    closegraph();
    return 0;
}