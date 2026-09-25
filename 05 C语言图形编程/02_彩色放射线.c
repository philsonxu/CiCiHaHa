#include "graphics.h"
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <dos.h>

int main()
{
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");

    int i;
    setbkcolor(0);
    //cleardevice();
    for(i = 0; i < 360; i += 5) {
        setcolor(i % 16);
        double rad = i * 3.14159 / 180;
        line(320, 240, 320 + 200 * cos(rad), 240 + 200 * sin(rad));
        // delay(20);
    }

    // getch();
    closegraph();
    return 0;
}