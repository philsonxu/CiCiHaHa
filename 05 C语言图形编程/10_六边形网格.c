#include "graphics.h"
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <dos.h>

int main()
{
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");

    int i, j, cx, cy;
    double r = 30, h = r * sqrt(3) / 2;
    setbkcolor(0);
    //cleardevice();
    for(i = -2; i < 8; i++) {
        for(j = -2; j < 6; j++) {
            cx = 150 + j * 2 * r * 0.75;
            cy = 120 + i * 2 * h;
            if(j % 2 != 0) cy += h;
            setcolor((i + j) % 14 + 1);
            for(int k = 0; k < 6; k++) {
                double a1 = k * 60 * 3.14159 / 180;
                double a2 = (k+1) * 60 * 3.14159 / 180;
                line(cx + r * cos(a1), cy + r * sin(a1),
                     cx + r * cos(a2), cy + r * sin(a2));
            }
        }
    }

    // getch();
    closegraph();
    return 0;
}