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
    setbkcolor(BLACK);
    //cleardevice();
    for(i = 0; i < 300; i++) {
        putpixel(random(640), random(480), WHITE);
    }
    for(j = 0; j < 50; j++) {
        setcolor(WHITE);
        for(i = 0; i < 20; i++) {
            putpixel(100 + j*8 - i*2, 100 + j*6 - i, YELLOW - i/2);
        }
        // delay(50);
        setcolor(BLACK);
        for(i = 0; i < 20; i++) {
            putpixel(100 + j*8 - i*2, 100 + j*6 - i, BLACK);
        }
    }

    // getch();
    closegraph();
    return 0;
}