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
    for(i = 0; i < 50; i++) {
        int x1 = random(640), y1 = random(480);
        int x2 = random(640), y2 = random(480);
        int x3 = random(640), y3 = random(480);
        setcolor(random(15) + 1);
        line(x1, y1, x2, y2);
        line(x2, y2, x3, y3);
        line(x3, y3, x1, y1);
    }

    // getch();
    closegraph();
    return 0;
}