/* 运输公司对用户计算运输费用。路程越远，运费越低，标准如下：
	s<250            没有折扣
	250<=s<500       2%折扣
	500<=s<1000      5%折扣
	1000<=s<2000     8%折扣
	2000<=s<3000     10%折扣
	3000<=s          15%折扣
*/

/*
分析： 每吨每千米货物的基本运费为p(price)，货物重为w(weight)，距离为s，折扣为d(discount)，
总运费f(freight)的计算公式为f=p*w*s(1-d)

经分析发现折扣的变化是有规律的，折扣的变化点都是250的倍数(250,500,1000,2000,3000)
用c表示250的倍数。c<1时，s<250，无折扣；1<=c<2时，表示250<=s<500，折扣d=2%，2<=c<4时，d=5%，
4<=c<8时，d=8%，8<=c<12时，d=10%，c>=12时，d=15%
*/
#include<stdio.h> 
int main()
{
	int c, s;
	float p, w, d, f;
	printf("please enter price, weight, discount:");// 提示输入的数据
	scanf("%f,%f,%d", &p, &w, &s);// 输入单间、重量、距离
	if (s >= 3000) c = 12; //3000km以上为同一折扣
	else c = s / 250;
	switch (c)
	{
	case 0:d = 0; break;
	case 1:d = 2; break;
	case 2:
	case 3:d = 5; break;
	case 4:
	case 5:
	case 6:
	case 7:d = 8; break;
	case 8:
	case 9:
	case 10:
	case 11:d = 10; break;
	case 12:d = 15; break;
	}

	f = p * w * s * (1 - d / 100);
	printf("freight=%10.2f\n", f);
	return 0;
}

