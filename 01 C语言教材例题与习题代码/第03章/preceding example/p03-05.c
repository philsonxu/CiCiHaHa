// 求ax^2+bx+c=0方程的根。a,b,c由键盘输入，设b^2-4ac>0。

#include<stdio.h> 
#include<math.h>
int main()
{
	double a, b, c, disc, x1, x2, p, q; // disc用来存放判别式(b*b-4ac)的值。
	scanf("%lf%lf%lf", &a, &b, &c); // 输入双精度型变量的值要用格式声明"%lf" 
	disc = b * b - 4 * a * c;
	p = -b / (2.0 * a);
	q = sqrt(disc) / (2.0 * a);
	x1 = p + q;
	x2 = p - q;
	printf("x1=%7.2lf\nx2=%7.2lf\n", x1, x2);// 7.2指定数据占7列，其中小数占2列 。 
	return 0;
}

