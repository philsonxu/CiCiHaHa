#include "graphics.h"
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int main()
{

	int gd = DETECT, gm;
	initgraph(&gd, &gm, "");
	setbkcolor(0);

	int h, m, s, xh, yh, xm, ym, xs, ys;
	//time_t t;
	time_t rawtime;
	struct tm* timeinfo;

	animate();
	int loop = 0;
	for (loop = 0; loop < 100; loop++)
	{
		framebegin();

		time(&rawtime);
		timeinfo = localtime(&rawtime);

		h = timeinfo->tm_hour % 12;
		m = timeinfo->tm_min;
		s = timeinfo->tm_sec;

		setcolor(WHITE);
		circle(320, 240, 150);
		setfillstyle(SOLID_FILL, WHITE);
		for (int i = 1; i <= 12; i++) {
			double a = (i * 30 - 90) * 3.14159 / 180;
			outtextxy(320 + 130 * cos(a) - 5, 240 + 130 * sin(a) - 5, "0" + i);
		}
		xs = 320 + 120 * cos((s * 6 - 90) * 3.14159 / 180);
		ys = 240 + 120 * sin((s * 6 - 90) * 3.14159 / 180);
		xm = 320 + 100 * cos((m * 6 - 90) * 3.14159 / 180);
		ym = 240 + 100 * sin((m * 6 - 90) * 3.14159 / 180);
		xh = 320 + 70 * cos((h * 30 + m * 0.5 - 90) * 3.14159 / 180);
		yh = 240 + 70 * sin((h * 30 + m * 0.5 - 90) * 3.14159 / 180);
		setcolor(RED); line(320, 240, xs, ys);
		setcolor(GREEN); line(320, 240, xm, ym);
		setcolor(YELLOW); line(320, 240, xh, yh);
		frameend();
	}

	closegraph();
	return 0;
}