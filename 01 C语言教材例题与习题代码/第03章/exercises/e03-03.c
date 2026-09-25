#include<stdio.h>
#include <math.h>

int main()
{
	float d = 300000, p = 6000, r = 0.01, m;
	m = log10(p / (p - d*r)) / log10(1 + r);
	printf("m = %3.1f\n", m);
	return 0;
}

