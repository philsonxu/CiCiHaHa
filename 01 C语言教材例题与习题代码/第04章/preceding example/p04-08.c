// 判断某一年是否为闰年

#include<stdio.h> 
int main()
{
	int year, leap;
	printf("enter year:");
	scanf("%d", &year);


	if (year % 4 == 0)
	{
		if (year % 100 == 0)
		{
			if (year % 400 == 0)
				leap = 1;
			else
				leap = 0;
		}
		else
			leap = 1;
	}
	else
		leap = 0;



	if (leap)
		printf("%d is", year);
	else
		printf("%d is not ", year);
	printf("a leap year.\n");
	return 0;
}

// 11-24可以改写为
/*
if(year%4!=0)
	leap=0;
else if (year%100!=0)
	leap=1;
else if(year%400!=0)
	leap=0;
else
	leap=1;
*/

//或者
/*
if((year%4==0&&year%100!=0)||(year%400==0))
	leap=1;
else
	leap=0;
*/

