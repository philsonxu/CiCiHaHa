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
	double angle = 0;
	int px, py;
	setbkcolor(0);
	animate();
	int loop = 0;
	for (loop = 0; loop < 100; loop++)
	{
		framebegin();
		setcolor(LIGHTBLUE);
		for (i = 0; i < 18; i++) 
		{
			double lat = (i - 9) * 10 * 3.14159 / 180;
			for (j = 0; j <= 36; j++) 
			{
				double lon = j * 10 * 3.14159 / 180 + angle;
				double x = 150 * cos(lat) * sin(lon);
				double y = 150 * sin(lat);
				double z = 150 * cos(lat) * cos(lon);
				double scale = 300 / (300 + z);
				int sx = 320 + x * scale;
				int sy = 240 + y * scale;
				if (j > 0) line(px, py, sx, sy);
				px = sx; py = sy;
			}
		}
		angle += 0.02;
		frameend();
	}

	// getch();
	closegraph();
	return 0;
}