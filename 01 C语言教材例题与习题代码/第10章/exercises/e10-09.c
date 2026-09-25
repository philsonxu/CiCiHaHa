#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct employee {
	int  num;      // 编号
	char name[32];
	char sex[4];
	int  age;
	char addr[60];
	int  salary;
	char health[10]; // 健康状况
	char class[10];  // 文化程度
};

struct emp {
	char name[32];
	int salary;
};

int main()
{
	int i;
	FILE* fp1, * fp2;
	struct emp emp_arr[5];
	struct employee employee_arr[5];

	fp1 = fopen("employee", "rb");
	fread(employee_arr, sizeof(employee_arr), 1, fp1);
	fclose(fp1);

	for (i = 0; i < 5; i++) {
		strcpy(emp_arr[i].name, employee_arr[i].name);
		emp_arr[i].salary = employee_arr[i].salary;
	}

	fp2 = fopen("emp", "wb");
	fwrite(emp_arr, sizeof(emp_arr), 1, fp2);
	fclose(fp2);
}

