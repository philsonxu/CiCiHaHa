#include "graphics.h"
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <dos.h>

int main()
{
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");

    int i, sides = 3;
    setbkcolor(0);
    //cleardevice();
    for(i = 0; i < 10; i++) {
        setcolor(i + 1);
        int r = 30 + i * 15;
        for(int j = 0; j < sides; j++) {
            double a1 = j * 2 * 3.14159 / sides + i * 0.1;
            double a2 = (j+1) * 2 * 3.14159 / sides + i * 0.1;
            line(320 + r * cos(a1), 240 + r * sin(a1),
                 320 + r * cos(a2), 240 + r * sin(a2));
        }
        sides++;
    }

    // getch();
    closegraph();
    return 0;
}