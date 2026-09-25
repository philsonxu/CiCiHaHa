#include "graphics.h"
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <dos.h>

int main()
{
	int gd = DETECT, gm;
	initgraph(&gd, &gm, "");

	int x, z, t = 0;
	setbkcolor(0);
	animate();
	int loop = 0;
	for (loop = 0; loop < 100; loop++)
	{
		framebegin();
		for (z = -150; z < 150; z += 10) {
			for (x = -200; x < 200; x += 5) {
				double y = 30 * sin((x + t) * 0.02) * cos((z + t) * 0.02);
				double scale = 300 / (300 + z);
				int sx = 320 + x * scale;
				int sy = 240 + y * scale + z * 0.3;
				putpixel(sx, sy, (int)(y + 40) / 5 + 1);
			}
		}
		t += 5;
		frameend();
	}

	// getch();
	closegraph();
	return 0;
}