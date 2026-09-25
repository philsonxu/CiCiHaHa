#include <stdio.h>
#include <stdlib.h>

struct student {
	int num;
	char name[32];
	int score[3];
	float avg;
};

void sort(struct student stu[], int len)
{
	int i, j;
	struct student tmp;
	for (i = 0; i < len; i++) {
		int min = i;
		for (j = i + 1; j < len; j++) {
			if (stu[j].avg > stu[min].avg)
				min = j;
		}
		tmp = stu[min];
		stu[min] = stu[i];
		stu[i] = tmp;
	}
}

int main()
{
	//int i;
	struct student stu[5];
	FILE* fp = NULL;
	if ((fp = fopen("stud", "rb")) == NULL) {
		printf("open file stud for read error\n");
		return 1;
	}

	if (fread(stu, sizeof(stu), 1, fp) != 1) {
		printf("write error\n");
		return 1;
	}
	fclose(fp);

	sort(stu, 5);

	FILE* fw = fopen("stu_sort", "wb");
	fwrite(stu, sizeof(stu), 1, fw);
	fclose(fw);
}

