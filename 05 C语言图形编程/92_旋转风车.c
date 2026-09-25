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
	setbkcolor(LIGHTBLUE);
	animate();
	int loop = 0;
	for (loop = 0; loop < 100; loop++)
	{
		framebegin();
		setcolor(BROWN);
		setfillstyle(SOLID_FILL, BROWN);
		bar(315, 240, 325, 450);
		for (i = 0; i < 4; i++) {
			double a = angle + i * 3.14159 / 2;
			int x = 320 + 100 * cos(a);
			int y = 240 + 100 * sin(a);
			setcolor(i + 1);
			setfillstyle(SOLID_FILL, i + 1);
			line(320, 240, x, y);
			int x2 = 320 + 80 * cos(a + 0.3);
			int y2 = 240 + 80 * sin(a + 0.3);
			line(320, 240, x2, y2);
			line(x, y, x2, y2);
			floodfill((x + x2) / 2, (y + y2) / 2, i + 1);
		}
		setfillstyle(SOLID_FILL, WHITE);
		circle(320, 240, 20);
		floodfill(320, 240, WHITE);
		angle += 0.1;
		frameend();
	}

	// getch();
	closegraph();
	return 0;
}