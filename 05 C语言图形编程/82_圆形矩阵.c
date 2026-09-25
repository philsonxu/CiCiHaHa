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
    for(i = 0; i < 8; i++) {
        for(j = 0; j < 6; j++) {
            setcolor((i + j) % 16);
            circle(70 + i * 70, 60 + j * 70, 25);
        }
    }

    // getch();
    closegraph();
    return 0;
}