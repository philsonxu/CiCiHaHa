#include "graphics.h"
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <dos.h>

int main()
{
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");

    double theta, r, x, y;
    int i;
    setbkcolor(0);
    //cleardevice();
    for(i = 0; i < 1000; i++) {
        theta = i * 0.05;
        r = 5 * theta;
        x = 320 + r * cos(theta);
        y = 240 + r * sin(theta);
        if(x >= 0 && x < 640 && y >= 0 && y < 480)
            putpixel((int)x, (int)y, i % 16);
    }

    // getch();
    closegraph();
    return 0;
}