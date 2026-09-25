// 阶跃函数

#include<stdio.h> 
int main()
{
	int x, y;
	scanf("%d", &x);
	if (x < 0)
		y = -1;
	else
		if (x == 0)y = 0;
		else y = 1;
	printf("x=%d,y=%d\n", x, y);
	return 0;
}
// 可以改写为：
/*
int main()
{
	intx,y;
	scanf("%d",&x);
	if(x>=0)
		if(x>0) y=1;
		esle y=0;
	else y=-1;
	printf("x=%d,y=%d\n",x,y);
	return 0;
}
*/

