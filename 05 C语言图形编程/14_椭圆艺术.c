#include "graphics.h"
#include <stdlib.h>
#include <math.h>

int main()
{
	int gd = DETECT, gm;
	initgraph(&gd, &gm, "");

	int i;
	setbkcolor(0);
	//cleardevice();
	for (i = 0; i < 360; i += 10) {
		setcolor(i / 10 % 16);
		ellipse(320, 240, 0, 360, 180, abs(120 * sin(i * 3.14159 / 180)));
	}

	// getch();
	closegraph();
	return 0;
}
