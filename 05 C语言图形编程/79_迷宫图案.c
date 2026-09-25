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
    for(i = 0; i < 20; i++) {
        for(j = 0; j < 30; j++) {
            if(random(3) > 0)
                line(20 + j * 20, 20 + i * 20, 40 + j * 20, 20 + i * 20);
            if(random(3) > 0)
                line(20 + j * 20, 20 + i * 20, 20 + j * 20, 40 + i * 20);
        }
    }

    // getch();
    closegraph();
    return 0;
}