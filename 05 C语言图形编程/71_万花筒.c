#include "graphics.h"
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <dos.h>

int main()
{
	int gd = DETECT, gm;
	initgraph(&gd, &gm, "");

	int i, r, g, b;
	double angle;
	setbkcolor(0);
	animate();
	int loop = 0;
	for (loop = 0; loop < 100; loop++)
	{
		framebegin();
		for (i = 0; i < 1000; i++) {
			angle = random(360) * 3.14159 / 180;
			r = random(200);
			int x = 320 + r * cos(angle);
			int y = 240 + r * sin(angle);
			int c = random(15) + 1;
			for (int k = 0; k < 8; k++) {
				double a = angle + k * 3.14159 / 4;
				putpixel(320 + r * cos(a), 240 + r * sin(a), c);
				putpixel(320 - r * cos(a), 240 + r * sin(a), c);
			}
		}
		frameend();
	}

	// getch();
	closegraph();
	return 0;
}