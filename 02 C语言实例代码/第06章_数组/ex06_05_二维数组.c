/*
 * 例6.5 二维数组的定义和使用
 * 知识点：二维数组按行存储、行列下标访问、矩阵输入输出
 */
#include <stdio.h>

int main()
{
    int a[3][4] = {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}};
    int i, j;
    
    printf("二维数组输出（3行4列矩阵）：\n");
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 4; j++) {
            printf("%-4d", a[i][j]);
        }
        printf("\n");
    }
    
    /* 求所有元素的和 */
    int sum = 0;
    int max = a[0][0];
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 4; j++) {
            sum += a[i][j];
            if (a[i][j] > max)
                max = a[i][j];
        }
    }
    printf("\n所有元素之和：%d\n", sum);
    printf("最大值：%d\n", max);
    
    return 0;
}