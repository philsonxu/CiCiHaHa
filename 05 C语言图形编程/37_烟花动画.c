#include "graphics.h"
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <dos.h>

int main()
{
	int gd = DETECT, gm;
	initgraph(&gd, &gm, "");

	int i, j, x, y;
	setbkcolor(BLACK);

	animate();
	int loop = 0;
	for (loop = 0; loop < 100; loop++)
	{
		framebegin();
		x = 100 + random(440);
		y = 100 + random(200);
		int color = random(15) + 1;
		for (i = 0; i < 360; i += 15) {
			setcolor(color);
			double rad = i * 3.14159 / 180;
			for (j = 0; j < 40; j++) {
				putpixel(x + j * cos(rad), y + j * sin(rad), color);
			}
		}
		frameend();
	}

	closegraph();
	return 0;
}