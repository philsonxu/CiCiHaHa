/*
 * 例6.6 矩阵转置
 * 知识点：二维数组操作、对称位置交换
 */
#include <stdio.h>

int main()
{
    int a[3][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    int i, j, t;
    
    printf("原矩阵：\n");
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            printf("%-4d", a[i][j]);
        }
        printf("\n");
    }
    
    /* 转置：a[i][j] <-> a[j][i] */
    for (i = 0; i < 3; i++) {
        for (j = i + 1; j < 3; j++) {  /* 只交换上三角 */
            t = a[i][j];
            a[i][j] = a[j][i];
            a[j][i] = t;
        }
    }
    
    printf("转置后矩阵：\n");
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            printf("%-4d", a[i][j]);
        }
        printf("\n");
    }
    
    return 0;
}