#include "graphics.h"
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <dos.h>
#include <time.h>

int main()
{

	int gd = DETECT, gm;
	initgraph(&gd, &gm, "");

	int i, angle = 0;

	time_t rawtime;
	struct tm* timeinfo;

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	setbkcolor(0);
	animate();
	int loop = 0;
	for (loop = 0; loop < 100; loop++)
	{
		framebegin();
		//gettime(&t);
		setcolor(GREEN);
		circle(320, 240, 200);
		for (i = 1; i <= 12; i++) {
			double a = i * 30 * 3.14159 / 180;
			outtextxy(320 + 180 * cos(a) - 5, 240 + 180 * sin(a) - 5, "0" + i);
		}
		double rad = angle * 3.14159 / 180;
		setcolor(LIGHTGREEN);
		line(320, 240, 320 + 200 * cos(rad), 240 + 200 * sin(rad));
		angle = (angle + 2) % 360;
		// delay(50);
		if (timeinfo->tm_sec % 2 == 0) {
			setcolor(BLACK);
			line(320, 240, 320 + 200 * cos((angle - 4) * 3.14159 / 180),
				240 + 200 * sin((angle - 4) * 3.14159 / 180));
		}
		frameend();
	}

	// getch();
	closegraph();

	return 0;
}