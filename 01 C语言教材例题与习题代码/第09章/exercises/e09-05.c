#include <stdio.h>

#define NAMLEN 20
#define STUCNT 10

typedef struct student_t {
	int num;
	char name[NAMLEN];
	int score[3];
} student;

int main() {
	student students[STUCNT];
	int maxi = 0, maxsum = 0;
	double aver_0 = 0, aver_1 = 0, aver_2 = 0;
	for (int i = 0; i < STUCNT; i++) {
		scanf("%d%s%d%d%d", &students[i].num, students[i].name, &students[i].score[0], &students[i].score[1], &students[i].score[2]);
		int sum = students[i].score[0] + students[i].score[1] + students[i].score[2];
		if (sum > maxsum) {
			maxsum = sum;
			maxi = i;
		}
		aver_0 += students[i].score[0];
		aver_1 += students[i].score[1];
		aver_2 += students[i].score[2];
	}
	aver_0 /= STUCNT;
	aver_1 /= STUCNT;
	aver_2 /= STUCNT;
	printf("%lf %lf %lf\n", aver_0, aver_1, aver_2);
	printf("%d %s %d %d %d %lf\n", students[maxi].num, students[maxi].name, students[maxi].score[0], students[maxi].score[1], students[maxi].score[2],
		(students[maxi].score[0] + students[maxi].score[1] + students[maxi].score[2]) / 3.0);
	return 0;
}

