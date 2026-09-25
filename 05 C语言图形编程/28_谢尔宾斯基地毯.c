#include "graphics.h"
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <dos.h>

void carpet(int x, int y, int size, int n)
{
    if (n == 0) return;
    int s = size / 3;
    setfillstyle(SOLID_FILL, n % 16);
    bar(x + s, y + s, x + 2 * s, y + 2 * s);
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (i != 1 || j != 1)
                carpet(x + i * s, y + j * s, s, n - 1);
}

int main()
{
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");


    setbkcolor(0);
    //cleardevice();
    setcolor(WHITE);
    rectangle(70, 70, 570, 470);
    carpet(70, 70, 500, 5);

    // getch();
    closegraph();
    return 0;
}