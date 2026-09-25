#include "graphics.h"
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <dos.h>

int main()
{
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");

    int i, x, y, brightness;
    setbkcolor(BLACK);
    //cleardevice();
    for(i = 0; i < 300; i++) {
        x = random(640);
        y = random(480);
        brightness = random(15) + 1;
        setcolor(brightness);
        if(random(10) > 7) {
            circle(x, y, 2);
            setfillstyle(SOLID_FILL, brightness);
            floodfill(x, y, brightness);
        } else {
            putpixel(x, y, brightness);
        }
    }
    setfillstyle(SOLID_FILL, YELLOW);
    circle(550, 80, 40);
    floodfill(550, 80, YELLOW);

    // getch();
    closegraph();
    return 0;
}