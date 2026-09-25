#include <stdio.h>
int main()
{
	void search(float(*p)[4], int n);
	float s[3][4] = { {65,67,70,60},{80,87,90,81},{90,99,100,98} };
	search(s, 3);
}
void search(float(*p)[4], int n)
{
	int i, j;
	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 4; j++)
			if (*(*(p + i) + j) < 70)
			{
				printf("No.%d fails,his score are:\n");
				for (j = 0; j < 4; j++)
					printf("%5.2f ", *(*(p + i) + j));
			}
	}
}
