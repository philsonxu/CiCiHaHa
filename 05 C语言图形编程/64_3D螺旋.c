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
	double angle = 0;
	setbkcolor(0);
	animate();
	int loop = 0;
	for (loop = 0; loop < 100; loop++)
	{
		framebegin();
		for (i = 0; i < 200; i++) {
			double t = i * 0.1;
			double x = 100 * cos(t + angle);
			double y = 100 * sin(t + angle);
			double z = i * 1.5 - 150;
			double scale = 300 / (300 + z);
			int sx = 320 + x * scale;
			int sy = 240 + y * scale;
			putpixel(sx, sy, i % 16);
		}
		angle += 0.05;
		frameend();
	}

	// getch();
	closegraph();
	return 0;
}