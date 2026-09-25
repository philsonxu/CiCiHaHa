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
	setbkcolor(0);
	animate();
	int loop = 0;
	for (loop = 0; loop < 100; loop++)
	{
		framebegin();
		int base[4][2];
		int apex[2];
		for (i = 0; i < 4; i++) {
			double a = angle + i * 3.14159 / 2;
			base[i][0] = 320 + 100 * cos(a);
			base[i][1] = 320 + 50 * sin(a);
		}
		apex[0] = 320; apex[1] = 120;
		setcolor(YELLOW);
		for (i = 0; i < 4; i++) {
			line(apex[0], apex[1], base[i][0], base[i][1]);
			line(base[i][0], base[i][1], base[(i + 1) % 4][0], base[(i + 1) % 4][1]);
		}
		angle += 0.02;
		frameend();
	}

	// getch();
	closegraph();
	return 0;
}