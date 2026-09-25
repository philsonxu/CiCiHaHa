#include "graphics.h"
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <dos.h>

int main()
{
	int gd = DETECT, gm;
	initgraph(&gd, &gm, "");

	int i, y;
	setbkcolor(0);
	animate();
	int loop = 0;
	for (loop = 0; loop < 100; loop++)
	{
		framebegin();
		for (i = 0; i < 100; i++) {
			int x = 270 + random(100);
			y = random(400) + 40;
			putpixel(x, y, random(15) + 1);
			putpixel(x, y + 1, BLUE);
			putpixel(x, y - 1, LIGHTBLUE);
		}
		setcolor(0);
		for (i = 0; i < 50; i++) {
			putpixel(270 + random(100), 440, BLACK);
		}
		frameend();
	}

	// getch();
	closegraph();
	return 0;
}