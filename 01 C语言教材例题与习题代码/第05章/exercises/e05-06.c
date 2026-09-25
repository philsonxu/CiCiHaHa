// 求\sum_{n=1}^{20} n!（即求1!+2!+3!+4!+...+20!） 。

#include<stdio.h> 
int main()
{
	double s = 0, t = 1;
	int n;
	for (n = 1; n <= 20; n++)
	{
		t = t * n;
		s = s + t;
	}
	printf("1!+2!+3!+4!+...+20!=%22.15e\n", s);
	return 0;
}

