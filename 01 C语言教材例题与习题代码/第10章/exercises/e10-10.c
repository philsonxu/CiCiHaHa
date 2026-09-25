#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct emp {
	char name[32];
	int salary;
};

int main()
{
	int i;
	FILE* fp;
	char name[32];
	struct emp emp_arr[5];

	fp = fopen("emp", "rb");
	fread(emp_arr, sizeof(emp_arr), 1, fp);
	fclose(fp);

	printf("name:");
	scanf("%s", &name);
	fp = fopen("emp", "wb");
	for (i = 0; i < 5; i++) {
		if (strcmp(emp_arr[i].name, name) == 0) {
			continue;
		}
		fwrite(&emp_arr[i], sizeof(emp_arr[i]), 1, fp);
	}
	fclose(fp);
}

