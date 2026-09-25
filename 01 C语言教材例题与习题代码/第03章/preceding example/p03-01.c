//华氏法转换为摄氏法 c=5/9(f-32)
#include<stdio.h> 
int main()
{
	float f, c; // 定义f和c为单精度浮点类型变量
	f = 64.0;
	c = (5.0 / 9) * (f - 32);
	printf("f=%f\nc=%f\n", f, c);
	return 0;
}

