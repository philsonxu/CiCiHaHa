/*
两个乒乓球队进行比赛，各出3人。甲队为A，B，C3人，乙队为X，Y，Z3人。
已抽签决定比赛名单。
有人向队员打听比赛的名单，A说他不和X比，C说他不和X，Z比，请编程序找出3对赛手的名单。
*/

#include<stdio.h>
int main()
{
	char i, j, k; // i是a的对手，j是b的对手，k是c的对手
	for (i = 'x'; i <= 'z'; i++)
	{
		for (j = 'x'; j <= 'z'; j++)
		{
			if (i != j)
				for (k = 'x'; k <= 'z'; k++)
				{
					if (i != k && j != k)
						if (i != 'x' && k != 'x' && k != 'z')
						{
							printf("A--%c\nB--%c\nC--%c\n", i, j, k);
							return 0;
						}

				}
		}
	}
	return 0;
}

