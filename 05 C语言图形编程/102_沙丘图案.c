#include <math.h>
#include "graphics.h"

#define PI 3.1415926

/*
* Turbo c2.0画的沙丘图案
* https://blog.csdn.net/chenbingjy/article/details/143723785
*/
int main()
{
	int x, y, py;
	double a, b, c;
	int gdriver = VGA, gmode = VGAHI;
	initgraph(&gdriver, &gmode, "");// "c:\\tc20\\bgi");
	//cleardevice();
	setbkcolor(9);
	setcolor(14);
	for (py = 20; py <= 380; py += 3)
	{
		c = (py - 20) * 4 * PI / 360;
		b = PI * cos(c);
		for (a = 0; a <= 6 * PI; a += PI / 7)
		{
			x = 600 / (6 * PI) * a + 20;
			y = 10 * sin(a + cos(a) * PI - b) * cos(c) + py;
			if (a == 0) moveto(x, y);
			else
				lineto(x, y);
		}
	}
	//getch();
	closegraph();
}