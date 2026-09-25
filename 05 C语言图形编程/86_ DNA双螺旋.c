#include "graphics.h"
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <dos.h>

int main()
{
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");

    double t;
    int i;
    setbkcolor(0);
    //cleardevice();
    for(i = 0; i < 400; i++) {
        t = i * 0.1;
        int x1 = 320 + 80 * sin(t);
        int y1 = i + 40;
        int x2 = 320 - 80 * sin(t);
        int y2 = i + 40;
        setcolor(LIGHTRED);
        circle(x1, y1, 5);
        setcolor(LIGHTBLUE);
        circle(x2, y2, 5);
        if(i % 20 < 10) {
            setcolor(WHITE);
            line(x1, y1, x2, y2);
        }
    }

    // getch();
    closegraph();
    return 0;
}