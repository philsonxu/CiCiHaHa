#include<stdio.h>
#include<math.h>

//x1为第一个根，x2为第二个根
float x1, x2, disc, p, q;

void greater_than_zero(float a, float b)
{
	float m = sqrt(disc);
	x1 = (-b + sqrt(disc)) / (2 * a);
	x2 = (-b - sqrt(disc)) / (2 * a);
}

void equal_to_zero(float a, float b)
{
	x1 = x2 = (-b) / (2 * a);
}

void smaller_than_zero(float a, float b)
{
	p = -b / (2 * a);
	q = sqrt(-disc) / (2 * a);
}

int main()
{
	int a, b, c;
	printf("请输入 a b c:");
	scanf("%d %d %d", &a, &b, &c);

	printf("表达式为: %d*x^2+%d*x+%d = 0\n", a, b, c);
	disc = b * b - 4 * a * c;

	if (disc > 0)
	{
		greater_than_zero(a, b);
		printf("disc>0的根为: x1=%f    x2=%f\n", x1, x2);
	}
	else if (disc == 0)
	{
		equal_to_zero(a, b);
		printf("disc==0的根为:x1=%f    x2=%f\n", x1, x2);
	}
	else
	{
		smaller_than_zero(a, b);
		printf("disc<0的根为:x1=%f+%f x2=%f-%f\n", p, q, p, q);
	}
	return 0;
}

