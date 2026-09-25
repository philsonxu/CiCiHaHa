#include "graphics.h"
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <dos.h>

void koch(int x1, int y1, int x2, int y2, int n) {
	int x3 = x1 + (x2 - x1) / 3, y3 = y1 + (y2 - y1) / 3;
	int x4 = x1 + 2 * (x2 - x1) / 3, y4 = y1 + 2 * (y2 - y1) / 3;
	int mx = (x1 + x2) / 2, my = (y1 + y2) / 2;
	int dx = x4 - x3, dy = y4 - y3;
	int x5 = mx - dy * 0.866, y5 = my + dx * 0.866;
	if(n == 0) {
		line(x1, y1, x2, y2);
		return;
	}
	koch(x1, y1, x3, y3, n-1);
	koch(x3, y3, x5, y5, n-1);
	koch(x5, y5, x4, y4, n-1);
	koch(x4, y4, x2, y2, n-1);
}

int main()
{
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");

    setbkcolor(0);
    //cleardevice();
    setcolor(WHITE);
    koch(150, 150, 490, 150, 4);
    koch(490, 150, 320, 420, 4);
    koch(320, 420, 150, 150, 4);

    // getch();
    closegraph();
    return 0;
}