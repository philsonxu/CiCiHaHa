// 想输出以下的结果，用函数调用实现。
/*
*****************
How do you do!
*****************
*/

#include<stdio.h> 
int main()
{
	void print_star();
	void print_message();
	print_star();
	print_message();
	print_star();
	return 0;
}

void print_star()
{
	printf("*****************\n");
}

void print_message()
{
	printf("How do you do!\n");
}

