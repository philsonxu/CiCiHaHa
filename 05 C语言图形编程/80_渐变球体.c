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
    for(i = -100; i <= 100; i++) {
        for(j = -100; j <= 100; j++) {
            if(i*i + j*j <= 10000) {
                double dx = i / 100.0, dy = j / 100.0;
                double light = dx * 0.5 - dy * 0.5 + 0.5;
                int c = (int)(light * 15);
                if(c < 1) c = 1; if(c > 15) c = 15;
                putpixel(320 + i, 240 + j, c);
            }
        }
    }

    // getch();
    closegraph();
    return 0;
}