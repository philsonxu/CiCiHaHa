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
    double p, q, r;
    double cx = -0.7, cy = 0.27015;
    setbkcolor(0);
    //cleardevice();
    for(y = 0; y < 480; y++) {
        for(x = 0; x < 640; x++) {
            p = (x - 320) / 160.0;
            q = (y - 240) / 160.0;
            for(k = 0; k < 100; k++) {
                r = p * p - q * q + cx;
                q = 2 * p * q + cy;
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