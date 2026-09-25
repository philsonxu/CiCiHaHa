#include "graphics.h"
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <dos.h>

int main()
{
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");

    int x, y, k;
    double p, q, x0, y0, r;
    setbkcolor(0);
    //cleardevice();
    for(y = 0; y < 480; y++) {
        for(x = 0; x < 640; x++) {
            p = 0; q = 0;
            x0 = (x - 320) / 160.0 - 0.5;
            y0 = (y - 240) / 160.0;
            for(k = 0; k < 100; k++) {
                r = p * p - q * q + x0;
                q = 2 * p * q + y0;
                p = r;
                if(p * p + q * q > 4) break;
            }
            if(k < 100) putpixel(x, y, k % 16);
        }
    }

    // getch();
    closegraph();
    return 0;
}