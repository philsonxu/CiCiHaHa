#include <stdio.h>
int main()
{
	int s[4][3] = { {10,80,90},{58,56,98},{99,88,69},{98,96,59} };
	int* search(int(*point)[3]);
	int* p, i, j, k;
	for (i = 0; i < 3; i++)
	{
		p = search(s + i);
		if (p == *(s + i))
		{
			k = i + 1;
			printf("No.%d score:", k);
			for (j = 0; j < 3; j++) printf("%d ", *(p + j));
		}
		printf("\n");
	}
}
int* search(int(*point)[3])
{
	int i, * pt;
	pt = NULL;
	for (i = 0; i < 3; i++)
		if (*(*point + i) < 60) pt = *point;
	return(pt);

}
