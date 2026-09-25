#include <stdio.h>

int main()
{
	int s[4][3] = { {70,80,90},{58,56,98},{99,88,69},{98,96,59} };
	int* search(int(*point)[3], int n);
	int* p, i, k;
	printf("enter the number of student:\n");
	scanf("%d", &k);
	p = search(s, k);
	for (i = 0; i < 3; i++) printf("%d ", *(p + i));
}
int* search(int(*point)[3], int n)
{
	int* p;
	p = (point + n);
	return(p);
}
