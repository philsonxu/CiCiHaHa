#include <stdio.h>
#include <stdlib.h>

void* mynew(int n)
{
	return malloc(n);
}
void myfree(char* p)
{
	return free(p);
}
int main()
{
	int num;
	char* str = NULL;
	printf("Please enter number: ");
	scanf_s("%d", &num);
	printf("before new p--%p:%s\n", str, str);//申请空间之前，查看指针的地址和指向空间数据
	str = (char*)mynew(num);
	printf("after new p--%p:%s\n", str, str);//申请空间之后，查看指针的地址和指向空间数据
	printf("Please enter a string:");
	scanf_s("%s", str, num);
	printf("before free p--%p:%s\n", str, str);//释放空间之前，查看指针的地址和指向空间数据
	myfree(str);
	printf("after free p--%p:%s\n", str, str);//释放空间之后，查看指针的地址和指向空间数据
	system("pause");
	return 0;
}

