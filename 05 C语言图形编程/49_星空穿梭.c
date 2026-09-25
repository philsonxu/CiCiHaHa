#include "graphics.h"
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <dos.h>

int main()
{
	int gd = DETECT, gm;
	initgraph(&gd, &gm, "");

	int i;
	double starx[300], stary[300], starz[300];
	setbkcolor(BLACK);
	animate();
	for (i = 0; i < 300; i++) {
		starx[i] = (random(640) - 320) / 320.0;
		stary[i] = (random(480) - 240) / 240.0;
		starz[i] = random(1000) / 100.0;
	}
	int loop = 0;
	for (loop = 0; loop < 100; loop++)
	{
		framebegin();
		for (i = 0; i < 300; i++) {
			starz[i] -= 0.5;
			if (starz[i] < 0.1) {
				starx[i] = (random(640) - 320) / 320.0;
				stary[i] = (random(480) - 240) / 240.0;
				starz[i] = 10;
			}
			int x = 320 + starx[i] / starz[i] * 300;
			int y = 240 + stary[i] / starz[i] * 240;
			int size = (10 - starz[i]) / 2;
			if (size < 1) size = 1;
			if (x >= 0 && x < 640 && y >= 0 && y < 480) {
				setcolor(15 - (int)starz[i]);
				circle(x, y, size);
			}
		}
		frameend();
	}

	// getch();
	closegraph();
	return 0;
}