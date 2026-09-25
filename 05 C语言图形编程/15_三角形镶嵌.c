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
    for (i = 0; i < 10; i++) {
        for (j = 0; j < 10; j++) {
            int x = 80 + j * 50;
            int y = 80 + i * 43;
            if ((i + j) % 2 == 0) {
                setcolor(RED);
                line(x, y + 43, x + 25, y);
                line(x + 25, y, x + 50, y + 43);
                line(x + 50, y + 43, x, y + 43);
            }
            else {
                setcolor(BLUE);
                line(x, y, x + 50, y);
                line(x + 50, y, x + 25, y + 43);
                line(x + 25, y + 43, x, y);
            }
        }
    }

    // getch();
    closegraph();
    return 0;
}