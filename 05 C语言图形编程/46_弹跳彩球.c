#include "graphics.h"
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <dos.h>

int main()
{
	int gd = DETECT, gm;
	initgraph(&gd, &gm, "");

	int i, n = 10;
	int x[10], y[10], dx[10], dy[10], color[10];
	setbkcolor(0);

	animate();
	for (i = 0; i < n; i++) {
		x[i] = random(500) + 70; y[i] = random(300) + 70;
		dx[i] = random(5) - 2; dy[i] = random(5) - 2;
		if (dx[i] == 0) dx[i] = 1; if (dy[i] == 0) dy[i] = 1;
		color[i] = i + 1;
	}
	int loop = 0;
	for (loop = 0; loop < 100; loop++)
	{
		framebegin();
		for (i = 0; i < n; i++) {
			setcolor(BLACK);
			circle(x[i], y[i], 15);
			x[i] += dx[i]; y[i] += dy[i];
			if (x[i] <= 15 || x[i] >= 625) dx[i] = -dx[i];
			if (y[i] <= 15 || y[i] >= 465) dy[i] = -dy[i];
			setcolor(color[i]);
			setfillstyle(SOLID_FILL, color[i]);
			circle(x[i], y[i], 15);
			floodfill(x[i], y[i], color[i]);
		}
		frameend();
	}

	// getch();
	closegraph();
	return 0;
}