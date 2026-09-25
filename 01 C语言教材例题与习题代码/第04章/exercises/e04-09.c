/*
给一个不多于5位的正整数，要求∶
①求出它是几位数;
②分别输出每一位数字;
③按逆序输出各位数字，例如原数为321，应输出123。
*/

#include<stdio.h>
#include<math.h>
int main()
{
	int num, indiv, ten, hundred, thousand, ten_thousand, place; // 分别代表个位、十位、百位、千位、万位和位数
	printf("请输入一个整数(0~99999):");
	scanf("%d", &num);
	if (num > 9999)
		place = 5;
	else if (num > 999)
		place = 4;
	else if (num > 99)
		place = 3;
	else if (num > 9)
		place = 2;
	else place = 1;
	printf("位数:%d\n", place);
	printf("每位数字为:");
	ten_thousand = num / 10000;
	thousand = (int)(num - ten_thousand * 10000) / 1000;
	hundred = (int)(num - ten_thousand * 10000 - thousand * 1000) / 100;
	ten = (int)(num - ten_thousand * 10000 - thousand * 1000 - hundred * 100) / 10;
	indiv = (int)(num - ten_thousand * 10000 - thousand * 1000 - hundred * 100 - ten * 10);
	switch (place)
	{
	case 5:
		printf("%d,%d,%d,%d,%d", ten_thousand, thousand, hundred, ten, indiv);
		printf("\n反序数字为:");
		printf("%d,%d,%d,%d,%d", indiv, ten, hundred, thousand, ten_thousand);
		break;
	case 4:
		printf("%d,%d,%d,%d", thousand, hundred, ten, indiv);
		printf("\n反序数字为:");
		printf("%d,%d,%d,%d", indiv, ten, hundred, thousand);
		break;
	case 3:
		printf("%d,%d,%d", hundred, ten, indiv);
		printf("\n反序数字为:");
		printf("%d,%d,%d", indiv, ten, hundred);
		break;
	case 2:
		printf("%d,%d", ten, indiv);
		printf("\n反序数字为:");
		printf("%d,%d", indiv, ten);
		break;
	case 1:
		printf("%d", indiv);
		printf("\n反序数字为:");
		printf("%d", indiv);
		break;
	}
	return 0;
}

