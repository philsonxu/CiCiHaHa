#include<stdio.h>

#define N 3

struct Student
{
	int num;
	char name[20];
	float score[3];
	float aver;
};

int main()
{
	void input(struct Student stu[]);
	struct Student max(struct Student stu[]);
	void print(struct Student stu);
	struct Student stu[N], * p = stu;
	input(p);
	print(max(p));
	return 0;
}

void input(struct Student stu[])
{
	int i;
	printf("请输入各学生的信息:学号、姓名、3门课成绩:\n");
	for (i = 0; i < N; i++)
	{
		scanf("%d %s %f %f %f", &stu[i].num, stu[i].name, &stu[i].score[0], &stu[i].score[1], &stu[i].score[2]);
		stu[i].aver = (float)((stu[i].score[0] + stu[i].score[1] + stu[i].score[2]) / 3.0);
	}
}

struct Student max(struct Student stu[])
{
	int i, m = 0;
	for (i = 1; i < N; i++)
	{
		if (stu[i].aver > stu[m].aver)
			m = i;
	}
	return stu[m];
}

void print(struct Student stud)
{
	printf("\n成绩最高的学生是:\n");
	printf("学号:%d\n姓名:%s\n三门课成绩:%5.1f，%5.1f，%5.1f\n平均成绩:%6.2f\n", stud.num, stud.name, stud.score[0], stud.score[1], stud.score[2], stud.aver);
}

