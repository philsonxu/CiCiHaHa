#include <stdio.h>
#include <malloc.h>

int** transform(int** arry, int row_count, int col_count)
{
	//列变行，行变列，则行的个数是以前列的个数，列的个数是以前行的个数
	int** p = NULL;
	//矩阵中有多少行，取决于以前有多少列，然后申请地址空间
	p = (int**)malloc(sizeof(int*) * col_count);
	for (int i = 0; i < col_count; i++) {
		//一行中有多少列，取决于以前有多少行，然后申请空间
		p[i] = (int*)malloc(sizeof(int) * row_count);
	}
	for (int i = 0; i < col_count; i++) {
		for (int j = 0; j < row_count; j++) {
			//二维数组的存储是扁平化的， 访问第j行第i列的数据，应该是 arry + j*列数 + i
			//j是新数组的列，但是是源数组的行
			p[i][j] = (arry + col_count * j)[i];
		}
	}
	return p;
}
int main()
{
	int arry[3][4];
	printf("Please enter a 3 x 4 matrix: \n");
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 4; j++) {
			scanf_s("%d", &arry[i][j]);
		}
	}
	int** p = transform(arry, 3, 4);
	printf("\n");
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 3; j++) {
			printf("%d ", p[i][j]);
		}
		free(p[i]); // 释放二级指针中每个一级指针申请的空间
		printf("\n");
	}
	free(p);//释放总体二级指针的空间
	system("pause");
	return 0;
}

