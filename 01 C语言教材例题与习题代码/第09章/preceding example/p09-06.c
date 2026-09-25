#include<stdio.h>
int main()
{
	struct student {
		char name[20];
		char sex[23];
		int age;
		int num;
	}a[3] = { {"zhao","nan",12,01},{"wang","nan",14,03},{"guo","nv",11,02} };
	struct student* p;
	p = a; int i;
	printf("姓名---性别---年龄---序号\n");
	for (i = 0; i < 3; i++, p++)
	{
		printf("%-5s  %-5s  %-5d  %-5d\n", p->name, p->sex, p->age, p->num);
	}
}
