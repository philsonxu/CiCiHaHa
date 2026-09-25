#include "graphics.h"
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <dos.h>

int main()
{
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");

    int i, x;
    setbkcolor(0);
    //cleardevice();
    setcolor(GREEN);
    for(x = 0; x < 640; x++) {
        int amp = 100 * sin(x * 0.02) * sin(x * 0.005);
        line(x, 240 - amp, x, 240 + amp);
    }

    // getch();
    closegraph();
    return 0;
}