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
		for (i = 0; i < 360; i += 10) {
			double a1 = (i + angle * 50) * 3.14159 / 180;
			double a2 = (i + angle * 50 + 30) * 3.14159 / 180;
			setcolor(i / 10 % 16);
			line(320 + 100 * cos(a1), 240 + 100 * sin(a1),
				320 + 100 * cos(a2), 240 + 100 * sin(a2));
			line(320 + 100 * cos(a1 + 3.14159 / 3), 240 + 100 * sin(a1 + 3.14159 / 3),
				320 + 100 * cos(a2 + 3.14159 / 3), 240 + 100 * sin(a2 + 3.14159 / 3));
			line(320 + 100 * cos(a1 + 2 * 3.14159 / 3), 240 + 100 * sin(a1 + 2 * 3.14159 / 3),
				320 + 100 * cos(a2 + 2 * 3.14159 / 3), 240 + 100 * sin(a2 + 2 * 3.14159 / 3));
		}
		angle += 0.01;
		frameend();
	}

	closegraph();
	return 0;
}