#include<stdio.h>
#include<stdlib.h>

struct Student_type
{
	char name[10];
	int num;
	int age;
	char addr[15];
}stud[10];

int main()
{
	int i;
	FILE* fp;
	if ((fp = fopen("stu.dat", "rb")) == NULL)
	{
		printf("can not open file\n");
		exit(0);
	}
	for (i = 0; i < 10; i += 2)
	{
		fseek(fp, i * sizeof(struct Student_type), 0);
		fread(&stud[i], sizeof(struct Student_type), 1, fp);
		printf("%-10s %4d %4d %-15s\n", stud[i].name, stud[i].num, stud[i].age, stud[i].addr);
	}
	fclose(fp);
	return 0;
}

