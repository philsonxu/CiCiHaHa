#include<stdio.h>//头文件
struct //定义结构体 
{
	int num;//编号 
	char name[20];//姓名 
	char sex;//性别 
	char job;//工作 
	union //联合体 
	{
		int class_Number;
		char position[10];
	} category; //共用体变量
}person[2];
int main()//主函数入口 
{
	int i;//定义整型变量 
	for (i = 0; i < 2; i++)//for循环2次 
	{
		printf("请输入数据:\n");//提示语句
		//输入信息 
		scanf("%d %s %c %c", &person[i].num, &person[i].name, &person[i].sex, &person[i].job); //person[i].name前面不加&照样可以运行
		if (person[i].job == 's')//如果工作是s 
		{
			scanf("%d", &person[i].category.class_Number);
		}
		else if (person[i].job == 't')//如果工作是s 
		{
			scanf("%s", &person[i].category.position);
		}
		else
		{
			printf("输入错误\n");//提示语句 
		}
	}
	printf("\n");//换行 
	printf("号码 名字 性别 工作 班级/职位\n");//提示语句 
	for (i = 0; i < 2; i++)//for循环2次 
	{
		if (person[i].job == 's')//如果工作是s 
		{
			//输出结果 
			printf("%-6d%-15s%-4c%-8c%-10d\n", person[i].num, person[i].name, person[i].sex, person[i].job, person[i].category.class_Number);
		}
		else
		{
			//输出结果 
			printf("%-6d%-15s%-4c%-8c%-10s\n", person[i].num, person[i].name, person[i].sex, person[i].job, person[i].category.position);
		}
	}
	return 0;//主函数返回值为0 
}

