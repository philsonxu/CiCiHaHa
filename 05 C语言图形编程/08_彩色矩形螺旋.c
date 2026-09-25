#include "graphics.h"
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <dos.h>

int main()
{
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");

    int i, x, y, size;
    setbkcolor(0);
    //cleardevice();
    x = 320; y = 240; size = 10;
    for(i = 0; i < 40; i++) {
        setcolor(i % 16);
        switch(i % 4) {
            case 0: line(x, y, x + size, y); x += size; break;
            case 1: line(x, y, x, y + size); y += size; break;
            case 2: line(x, y, x - size - 5, y); x -= size + 5; break;
            case 3: line(x, y, x, y - size - 5); y -= size + 5; break;
        }
        size += 5;
        // delay(50);
    }

    // getch();
    closegraph();
    return 0;
}