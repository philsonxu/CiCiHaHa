#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct student {
	int num;
	char name[32];
	int score[3];
	float avg;
};

int main()
{
	//int i;
	struct student stu[6];
	FILE* fp = NULL;
	if ((fp = fopen("tmp_sort", "rb")) == NULL) {
		printf("open file stud for read error\n");
		return 1;
	}

	if (fread(stu, sizeof(stu), 1, fp) != 1) {
		printf("write error\n");
		return 1;
	}
	fclose(fp);

	FILE* fw = fopen("stu_sort", "wb");
	fwrite(stu, sizeof(stu), 1, fw);
	fclose(fw);
}

