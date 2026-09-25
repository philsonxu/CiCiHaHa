/*
 * 例6.1 一维数组的定义、初始化和输入输出
 * 知识点：数组定义、数组下标从0开始、逐个访问数组元素
 */
#include <stdio.h>

int main()
{
    int i, a[10];  /* 定义含10个整型元素的数组 */
    
    /* 初始化方式1：逐个赋值 */
    for (i = 0; i < 10; i++) {
        a[i] = i * 2;
    }
    
    printf("数组元素为：\n");
    for (i = 0; i < 10; i++) {
        printf("a[%d] = %d\n", i, a[i]);
    }
    
    /* 初始化方式2：定义时初始化 */
    int b[5] = {1, 3, 5, 7, 9};
    printf("\n数组b的元素：");
    for (i = 0; i < 5; i++) {
        printf("%d ", b[i]);
    }
    printf("\n");
    
    /* 部分初始化，其余元素自动为0 */
    int c[10] = {1, 2, 3};
    printf("数组c的元素：");
    for (i = 0; i < 10; i++) {
        printf("%d ", c[i]);
    }
    printf("\n");
    
    return 0;
}