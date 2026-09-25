/*
用迭代法求x=sqrt(a)。求平方根的迭代公式为
x_{n+1}=\frac{1}{2}(x_n+\frac{a}{x_n})
要求前后两次求出的x的差的绝对值小于10^{-5}。
*/
/*
解∶用迭代法求平方根的算法如下∶
（1）设定一个x的初值x_0；
（2）用以上公式求出x的下一个值x_1；
（3）再将x1代入以上公式右侧的x_n，求出x的下一个值x_2；
（4）如此继续下去，直到前后两次求出的x值（x_n和x_{n+1}）满足以下关系∶
|x_{n+1}-x_n|<10^{-5}
为了便于程序处理，今只用x_0和x_1，先令x的初值x_0=a/2（也可以是另外的值），求出x1；
如果此时|x_1-x_0|≥10^{-5}，就使x_1→x。，然后用这个新的x。
求出下一个x1；如此反复，直到|x1-x0|<10^{-5}为止。
*/

#include <stdio.h>
#include <math.h> 
int main()
{
	float a, x0, x1;
	printf("enter a positive number:");
	scanf("%f", &a);
	x0 = a / 2;
	x1 = (x0 + a / x0) / 2;
	do
	{
		x0 = x1;
		x1 = (x0 + a / x0) / 2;
	} while (fabs(x0 - x1) >= 1e-5);
	printf("The square root of %5.2f is %8.5f\n", a, x1);
	return 0;
}

