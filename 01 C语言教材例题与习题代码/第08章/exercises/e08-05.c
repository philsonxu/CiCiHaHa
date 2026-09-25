#include <stdio.h>
int main()
{
	int people[128], n;
	printf("Please input how many people: ");
	scanf_s("%d", &n);
	for (int i = 0; i < n; i++) {
		people[i] = i + 1; //对每个人顺序排号
	}
	int remain = n;
	int num_off = 0;
	int* p = NULL;
	while (remain > 1) {
		p = people;
		while (p != people + n) { // 每次从第一个位置开始，直到最后一个位置,报数是一直递增的
			if ((*p) != 0) {//若这个位置人还在
				num_off++; //则报数
				if (num_off == 3) {//否则当前的人即将要报的数字是3
					*p = 0;  //则剔除这个人
					num_off = 0; //并且重新开始计数，下边会++，所以是从1开始报数
					remain--;//剩余人数-1
				}
			}
			p++;
		}
	}
	for (int i = 0; i < n; i++) {
		if (people[i] != 0) {
			printf("Serial number of the remaining person:%d\n", people[i]);
		}
	}

	printf("\n");
	system("pause");
	return 0;
}

