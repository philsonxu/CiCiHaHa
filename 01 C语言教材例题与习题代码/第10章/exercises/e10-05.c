#include <stdio.h>
#include <stdlib.h>

struct student {
	int num;
	char name[32];
	int score[3];
	float avg;
};

int main() {
	int i;
	struct student stu[5];
	FILE* fp = NULL;

	for (i = 0; i < 5; i++) {
		printf("num name score1 score2 score3:\n");
		scanf("%d %s %d %d %d", &stu[i].num, &stu[i].name,
			&stu[i].score[0], &stu[i].score[1], &stu[i].score[2]);
		stu[i].avg = (stu[i].score[0] + stu[i].score[1] + stu[i].score[2]) / 3.0;
	}

	if ((fp = fopen("stud", "wb")) == NULL) {
		printf("open file stud for write error\n");
		return 1;
	}

	if (fwrite(stu, sizeof(stu), 1, fp) != 1) {
		printf("write error\n");
		return 1;
	}
	fclose(fp);
}

