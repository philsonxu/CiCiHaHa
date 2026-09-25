#include "graphics.h"
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <dos.h>

int main()
{
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");

    int i, t;
    setbkcolor(BLUE);
    //cleardevice();
    for(t = 0; t < 200; t++) {
        setcolor(LIGHTBLUE);
        for(i = 0; i < 640; i++) {
            int y = 240 + 50 * sin((i + t * 5) * 3.14159 / 100);
            line(i, y, i, 480);
        }
        setfillstyle(SOLID_FILL, YELLOW);
        circle(100, 100, 30);
        floodfill(100, 100, YELLOW);
        // delay(50);
        setcolor(BLUE);
        for(i = 0; i < 640; i++) {
            int y = 240 + 50 * sin((i + t * 5) * 3.14159 / 100);
            line(i, y - 5, i, 480);
        }
    }

    // getch();
    closegraph();
    return 0;
}