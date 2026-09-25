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
	int px[] = { 0, -60, 60, 0 }, py[] = { -80, 40, 40, 40 };
	setbkcolor(0);
	animate();
	int loop = 0;
	for (loop = 0; loop < 100; loop++)
	{
		framebegin();
		setcolor(YELLOW);
		for (i = 0; i < 3; i++) {
			double x1 = 320 + px[i] * cos(angle) - 0 * sin(angle);
			double y1 = 240 + py[i];
			double x2 = 320 + px[i + 1] * cos(angle) - 0 * sin(angle);
			double y2 = 240 + py[i + 1];
			line((int)x1, (int)y1, 320 + px[0] * cos(angle), (int)(240 + py[0]));
			line((int)x1, (int)y1, (int)x2, (int)y2);
		}
		angle += 0.03;
		frameend();
	}

	// getch();
	closegraph();
	return 0;
}