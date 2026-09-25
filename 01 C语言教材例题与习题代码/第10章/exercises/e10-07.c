#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct student {
	int num;
	char name[32];
	int score[3];
	float avg;
};

void sort(struct student stu[], int len) {
	int i, j;
	struct student tmp;
	for (i = 0; i < len; i++) {
		int min = i;
		for (j = i + 1; j < len; j++) {
			if (stu[j].avg > stu[min].avg)
				min = j;
		}
		if (min != i) {
			tmp = stu[min];
			stu[min] = stu[i];
			stu[i] = tmp;
		}
	}
}
int main()
{
	//int i;
	struct student stu[5];
	FILE* fp = NULL;
	if ((fp = fopen("stu_sort", "rb")) == NULL) {
		printf("open file stud for read error\n");
		return 1;
	}

	if (fread(stu, sizeof(stu), 1, fp) != 1) {
		printf("write error\n");
		return 1;
	}
	fclose(fp);

	struct student new_stu[6];
	memcpy(new_stu, stu, sizeof(stu));
	printf("num name score0 score1 score2:\n");
	scanf("%d %s %d %d %d", &new_stu[5].num, &new_stu[5].name, &new_stu[5].score[0],
		&new_stu[5].score[1], &new_stu[5].score[2]);
	new_stu[5].avg = (new_stu[5].score[0] + new_stu[5].score[1] + new_stu[5].score[2]) / 3.0;
	sort(new_stu, 6);

	FILE* fw = fopen("tmp_sort", "wb");
	fwrite(new_stu, sizeof(new_stu), 1, fw);
	fclose(fw);
}

