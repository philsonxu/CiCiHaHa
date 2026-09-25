#include "graphics.h"
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <dos.h>

int main()
{
	int gd = DETECT, gm;
	initgraph(&gd, &gm, "");

	int i, j;
	double offset = 0;
	setbkcolor(0);
	animate();
	int loop = 0;
	for (loop = 0; loop < 100; loop++)
	{
		framebegin();
		for (i = 0; i < 16; i++) {
			for (j = 0; j < 12; j++) {
				int yoff = 20 * sin((i + offset) * 0.5);
				if ((i + j) % 2 == 0)
					setfillstyle(SOLID_FILL, WHITE);
				else
					setfillstyle(SOLID_FILL, BLACK);
				bar(10 + i * 40, 10 + j * 40 + yoff,
					45 + i * 40, 45 + j * 40 + yoff);
			}
		}
		offset += 0.2;
		frameend();
	}

	// getch();
	closegraph();
	return 0;
}