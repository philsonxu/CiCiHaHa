#include <stdio.h>

void main()
{
	enum color { red, yellow, blue, white, black };
	enum color i, j, k, pri;
	int n = 0, loop;

	for (i = red; i <= black; i++)
		for (j = red; j <= black; j++)
			if (i != j)
			{
				for (k = red; k <= black; k++)
				{
					if ((k != i) && (k != j))
					{
						n = n + 1;
						printf("%-4d", n);
						for (loop = 1; loop <= 3; loop++)
						{
							switch (loop)
							{
							case 1:pri = i; break;
							case 2:pri = j; break;
							case 3:pri = k; break;
							default:break;
							}

							switch (pri)
							{
							case red:printf("%-10s", "red"); break;
							case yellow:printf("%-10s", "yellow"); break;
							case blue:printf("%-10s", "blue"); break;
							case white:printf("%-10s", "white"); break;
							case black:printf("%-10s", "black"); break;
							default:break;
							}
						}
						printf("\n");
					}
				}
			}

	printf("\ntotal:%5d\n", n);
}
