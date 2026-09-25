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
	double theta;
	setbkcolor(0);

	animate();
	for (int frame = 0; frame < 360; frame += 5)
	{
		framebegin();
		for (i = 0; i < 720; i++) {
			theta = (i + frame) * 3.14159 / 180;
			double r = i / 4.0;
			putpixel(320 + r * cos(theta), 240 + r * sin(theta), i % 16);
		}
		frameend();
	}

	closegraph();
	return 0;
}