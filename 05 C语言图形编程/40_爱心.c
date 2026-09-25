#include "graphics.h"
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <dos.h>

int main()
{
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");

    double t;
    int x, y;
    setbkcolor(0);
    //cleardevice();
    setcolor(RED);
    for(t = 0; t < 2 * 3.14159; t += 0.001) {
        x = 320 + 16 * pow(sin(t), 3) * 8;
        y = 240 - (13 * cos(t) - 5 * cos(2*t) - 2 * cos(3*t) - cos(4*t)) * 8;
        putpixel(x, y, RED);
        for(int i = 0; i < 10; i++) {
            putpixel(x + random(5) - 2, y + random(5) - 2, LIGHTRED);
        }
    }

    // getch();
    closegraph();
    return 0;
}