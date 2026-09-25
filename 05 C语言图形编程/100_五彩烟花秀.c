#include "graphics.h"
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <dos.h>

int main()
{
	int gd = DETECT, gm;
	initgraph(&gd, &gm, "");

	int i, j, k;
	setbkcolor(BLACK);

	animate();
	int loop = 0;
	for (loop = 0; loop < 100; loop++)
	{
		framebegin();
		int x = random(540) + 50;
		int y = random(200) + 50;
		int color = random(15) + 1;
		int type = random(3);
		if (type == 0) {
			for (i = 0; i < 360; i += 10) {
				double rad = i * 3.14159 / 180;
				setcolor(color);
				for (j = 0; j < 50; j++) {
					putpixel(x + j * cos(rad), y + j * sin(rad), color);
				}
			}
		}
		else if (type == 1) {
			for (i = 0; i < 100; i++) {
				double angle = random(360) * 3.14159 / 180;
				double speed = random(50);
				setcolor(color);
				for (k = 0; k < 20; k++) {
					putpixel(x + speed * cos(angle) * k / 20,
						y + speed * sin(angle) * k / 20, color);
				}
			}
		}
		else {
			for (i = 1; i <= 5; i++) {
				setcolor((color + i) % 16);
				circle(x, y, i * 10);
			}
		}
		frameend();
	}

	closegraph();
	return 0;
}