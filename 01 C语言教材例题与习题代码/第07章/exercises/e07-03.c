#include<stdio.h>
#include<stdbool.h>

bool IsPrime(int value)
{
	for (int i = 2; i < value / 2; ++i)
	{
		if (value % i == 0) //说明除了1和本身之外，还能被其他数整除
			return false;
	}
	return true;
}

int main()
{
	int value;
	bool flag;
	printf("请输入 value :>");
	scanf("%d", &value);

	flag = IsPrime(value);
	if (flag)
		printf("%d 是素数.\n", value);
	else
		printf("%d 不是素数.\n", value);

	return 0;
}

