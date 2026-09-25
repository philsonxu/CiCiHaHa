#include <stdio.h>
#include<stdio.h>

/* 函数sum_day:计算日期 */
int sum_day(int month, int day)
{
	int day_tab[13] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	int i;
	for (i = 1; i < month; i++)
		day += day_tab[i];      /* 累加所在月之前天数 */
	return day;
}

/* 函数leap:判断是否为闰年 */
int leap(int year)
{
	int leap;
	leap = year % 4 == 0 && year % 100 != 0 || year % 400 == 0;
	return leap;
}

int main()
{
	int year, month, day, days;
	printf("input date(year,month,day):");
	scanf("%d %d %d", &year, &month, &day);
	printf("%d/%d/%d ", year, month, day);
	days = sum_day(month, day);                  /* 调用函数sum_day */
	if (leap(year) && month >= 3)                  /* 调用函数leap */
		days = days + 1;
	printf("is the %dth day in this year.\n", days);
	return 0;
}

