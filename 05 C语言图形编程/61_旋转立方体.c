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
	int v[8][3] = { {-50,-50,-50},{50,-50,-50},{50,50,-50},{-50,50,-50},
				   {-50,-50,50},{50,-50,50},{50,50,50},{-50,50,50} };
	int edges[12][2] = { {0,1},{1,2},{2,3},{3,0},{4,5},{5,6},{6,7},{7,4},
						{0,4},{1,5},{2,6},{3,7} };
	setbkcolor(0);
	animate();
	int loop = 0;
	for (loop = 0; loop < 100; loop++)
	{
		framebegin();
		int proj[8][2];
		for (i = 0; i < 8; i++) {
			double x = v[i][0] * cos(angle) - v[i][2] * sin(angle);
			double z = v[i][0] * sin(angle) + v[i][2] * cos(angle);
			double y = v[i][1];
			proj[i][0] = 320 + x;
			proj[i][1] = 240 + y;
		}
		for (i = 0; i < 12; i++) {
			setcolor(i + 1);
			line(proj[edges[i][0]][0], proj[edges[i][0]][1],
				proj[edges[i][1]][0], proj[edges[i][1]][1]);
		}
		angle += 0.03;
		frameend();
	}

	// getch();
	closegraph();
	return 0;
}