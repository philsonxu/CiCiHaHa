#include "graphics.h"
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <dos.h>

int main()
{
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");

    int i, j, x, y;
    setbkcolor(0);
    //cleardevice();
    for(i = 0; i < 20; i++) {
        for(j = 0; j < 20; j++) {
            x = i * 32 + 16;
            y = j * 24 + 12;
            if((i + j) % 2 == 0) {
                setcolor(WHITE);
                circle(x, y, 12);
            } else {
                setfillstyle(SOLID_FILL, WHITE);
                circle(x, y, 12);
                floodfill(x, y, WHITE);
            }
        }
    }

    // getch();
    closegraph();
    return 0;
}