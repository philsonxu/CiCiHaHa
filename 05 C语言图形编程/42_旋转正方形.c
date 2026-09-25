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
	int x[] = { -50, 50, 50, -50 }, y[] = { -50, -50, 50, 50 };
	setbkcolor(0);

	animate();
	int loop = 0;
	for (loop = 0; loop < 100; loop++)
	{
		framebegin();
		setcolor(LIGHTCYAN);
		for (i = 0; i < 4; i++) {
			int j = (i + 1) % 4;
			double x1 = 320 + x[i] * cos(angle) - y[i] * sin(angle);
			double y1 = 240 + x[i] * sin(angle) + y[i] * cos(angle);
			double x2 = 320 + x[j] * cos(angle) - y[j] * sin(angle);
			double y2 = 240 + x[j] * sin(angle) + y[j] * cos(angle);
			line((int)x1, (int)y1, (int)x2, (int)y2);
		}
		angle += 0.05;
		frameend();
	}

	closegraph();
	return 0;
}