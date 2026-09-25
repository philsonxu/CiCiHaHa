#include <stdio.h>
int main()
{
	void a(float* p, int n);
	void search(float(*p)[4], int n);
	float s[3][4] = { {65,67,70,60},{80,87,90,81},{90,99,100,98} };
	printf("please input one student’s No.:");
	int n;
	scanf("%d", &n);
	search(s, n);
	a(*s, 12);
}
void search(float(*p)[4], int n)
{
	int i;
	printf("The score of No.%d are: ");
	for (i = 0; i < 4; i++) printf("%5.2f ", *(*(p + n) + i));
	printf("\n");
}
void a(float* p, int n)
{
	float* p_e, sum, aver;
	p_e = p + n - 1;
	for (sum = 0; p <= p_e; p++) sum = sum + (*p);
	aver = sum / n;
	printf("average=%5.2f\n", aver);
}

