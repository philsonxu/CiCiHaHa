#include "graphics.h"
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <dos.h>

int main()
{
	int gd = DETECT, gm;
	initgraph(&gd, &gm, "");

	int i = 0, depth;
	setbkcolor(0);
	animate();
	int loop = 0;
	for (loop = 0; loop < 100; loop++)
	{
		framebegin();
		for (depth = 0; depth < 20; depth++) {
			int size = 20 + depth * 15;
			int offset = (depth * 20 + i) % 40;
			setcolor(depth % 16);
			rectangle(320 - size - offset, 240 - size * 3 / 4 - offset * 3 / 4,
				320 + size + offset, 240 + size * 3 / 4 + offset * 3 / 4);
		}
		i++;
		frameend();
	}

	// getch();
	closegraph();
	return 0;
}