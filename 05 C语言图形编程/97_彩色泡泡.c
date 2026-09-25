#include "graphics.h"
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <dos.h>

int main()
{
	int gd = DETECT, gm;
	initgraph(&gd, &gm, "");

	int i, x[30], y[30], r[30], dy[30], c[30];
	setbkcolor(0);
	animate();
	for (i = 0; i < 30; i++) {
		x[i] = random(600) + 20;
		y[i] = 480;
		r[i] = random(20) + 10;
		dy[i] = random(3) + 1;
		c[i] = random(15) + 1;
	}
	int loop = 0;
	for (loop = 0; loop < 100; loop++)
	{
		framebegin();
		for (i = 0; i < 30; i++) {
			setcolor(BLACK);
			circle(x[i], y[i], r[i]);
			y[i] -= dy[i];
			if (y[i] < -20) y[i] = 500;
			setcolor(c[i]);
			circle(x[i], y[i], r[i]);
		}
		frameend();
	}

	// getch();
	closegraph();
	return 0;
}