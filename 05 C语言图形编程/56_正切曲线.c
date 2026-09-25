#include "graphics.h"
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <dos.h>

int main()
{
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");

    int x;
    double y;
    setbkcolor(0);
    //cleardevice();
    setcolor(WHITE);
    line(50, 240, 590, 240);
    line(320, 50, 320, 430);
    setcolor(LIGHTRED);
    for(x = 0; x < 540; x++) {
        double val = tan((x - 270) * 3.14159 / 270);
        if(abs(val) < 5) {
            y = 240 - 40 * val;
            if(y >= 0 && y < 480) putpixel(50 + x, (int)y, LIGHTRED);
        }
    }

    // getch();
    closegraph();
    return 0;
}