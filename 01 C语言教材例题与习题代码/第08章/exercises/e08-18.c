#include<stdio.h>
int main()
{
	int month;
	char* Month[12] = { "January","February","March","April","May","June",
	"July","August","September","October","November","December" };
	while (1) {
		printf("Please enter the month: ");
		scanf_s("%d", &month);
		if (month < 1 && month>12) {
			printf("Input error, Month should be greater than 0 and less than 12\n");
		}
		printf("%s\n", Month[month - 1]);
	}
	return 0;
}


