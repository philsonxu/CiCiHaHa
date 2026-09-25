#include "graphics.h"
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <dos.h>

void h_fractal(int x, int y, int size, int n) 
{
    if (n == 0) return;
    setcolor(n % 16);
    line(x - size, y, x + size, y);
    line(x - size, y - size, x - size, y + size);
    line(x + size, y - size, x + size, y + size);
    // delay(10);
    h_fractal(x - size, y - size, size / 2, n - 1);
    h_fractal(x + size, y - size, size / 2, n - 1);
    h_fractal(x - size, y + size, size / 2, n - 1);
    h_fractal(x + size, y + size, size / 2, n - 1);
}

int main()
{
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");

    setbkcolor(0);
    //cleardevice();
    h_fractal(320, 240, 150, 6);

    // getch();
    closegraph();
    return 0;
}