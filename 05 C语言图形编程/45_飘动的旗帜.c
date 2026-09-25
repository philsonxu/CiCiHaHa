#include "graphics.h"
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <dos.h>

int main()
{
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");

    int i, j, offset;
    setbkcolor(LIGHTBLUE);
    //cleardevice();
    setfillstyle(SOLID_FILL, DARKGRAY);
    bar(150, 50, 160, 400);
    for(offset = 0; offset < 100; offset++) {
        setcolor(RED);
        for(i = 0; i < 200; i++) {
            for(j = 0; j < 120; j++) {
                int wave = 10 * sin((i + offset * 5) * 3.14159 / 50);
                putpixel(160 + i, 100 + j + wave * j / 120, RED);
            }
        }
        // delay(50);
        for(i = 0; i < 200; i++) {
            for(j = 0; j < 120; j++) {
                int wave = 10 * sin((i + offset * 5) * 3.14159 / 50);
                putpixel(160 + i, 100 + j + wave * j / 120, LIGHTBLUE);
            }
        }
    }

    // getch();
    closegraph();
    return 0;
}