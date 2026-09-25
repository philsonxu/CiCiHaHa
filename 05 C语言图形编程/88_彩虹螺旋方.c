#include "graphics.h"
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <dos.h>

int main()
{
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");

    int i, len = 5, x = 320, y = 240;
    setbkcolor(0);
    //cleardevice();
    for(i = 0; i < 60; i++) {
        setcolor(i % 16);
        switch(i % 4) {
            case 0: line(x, y, x + len, y); x += len; break;
            case 1: line(x, y, x, y - len); y -= len; break;
            case 2: line(x, y, x - len - 3, y); x -= len + 3; break;
            case 3: line(x, y, x, y + len + 3); y += len + 3; break;
        }
        len += 3;
        // delay(30);
    }

    // getch();
    closegraph();
    return 0;
}