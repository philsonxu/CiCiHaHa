#include "graphics.h"
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <dos.h>

int main()
{
	int gd = DETECT, gm;
	initgraph(&gd, &gm, "");

	int i, angle = 0;
	setbkcolor(0);
	animate();
	int loop = 0;
	for (loop = 0; loop < 100; loop++)
	{
		framebegin();
		setcolor(GREEN);
		circle(320, 240, 200);
		circle(320, 240, 150);
		circle(320, 240, 100);
		circle(320, 240, 50);
		line(120, 240, 520, 240);
		line(320, 40, 320, 440);
		double rad = angle * 3.14159 / 180;
		setcolor(LIGHTGREEN);
		for (i = 0; i < 200; i++) {
			putpixel(320 + i * cos(rad), 240 + i * sin(rad), LIGHTGREEN);
		}
		for (i = 0; i < 30; i++) {
			double r2 = rad - i * 0.02;
			setcolor(GREEN - i / 3);
			line(320, 240, 320 + 200 * cos(r2), 240 + 200 * sin(r2));
		}
		angle = (angle + 2) % 360;
		frameend();
	}

	// getch();
	closegraph();
	return 0;
}