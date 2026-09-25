/*
有一行电文，已按下面规律译成密码∶
A→Z a→z B→Y b→y C→X c→x :
即第1个字母变成第 26个字母，第i个字母变成第（26-i+1）个字母，非字母字符不变。要
求编程序将密码译回原文，并输出密码和原文。
*/
#include<stdio.h> 
int main()
{
	int j, n;
	char ch[80], tran[80];
	printf("input cipher code:");
	gets(ch);
	printf("\ncipher code  :%s", ch);
	j = 0;
	while (ch[j] != '\0')
	{
		if ((ch[j] >= 'A') && (ch[j] <= 'Z'))
			tran[j] = 155 - ch[j];
		else if ((ch[j]) >= 'a' && (ch[j] <= 'z'))
			tran[j] = 219 - ch[j];
		else
			tran[j] = ch[j];
		j++;
	}
	n = j;
	printf("\noriginal text:");
	for (j = 0; j < n; j++)
		putchar(tran[j]);
	printf("\n");
	return 0;
}

