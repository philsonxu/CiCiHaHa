#include "graphics.h"
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <dos.h>

int main()
{
	int gd = DETECT, gm;
	initgraph(&gd, &gm, "");

	int x = 100, y = 100, dx = 3, dy = 2;
	setbkcolor(0);

	animate();
	int loop = 0;
	for (loop = 0; loop < 100; loop++)
	{
		framebegin();
		setcolor(BLACK);
		circle(x, y, 20);
		x += dx; y += dy;
		if (x <= 20 || x >= 620) dx = -dx;
		if (y <= 20 || y >= 460) dy = -dy;
		setcolor(YELLOW);
		setfillstyle(SOLID_FILL, YELLOW);
		circle(x, y, 20);
		floodfill(x, y, YELLOW);
		frameend();
	}

	closegraph();
	return 0;
}