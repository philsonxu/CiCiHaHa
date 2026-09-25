/*
 * 例6.3 冒泡排序（升序）
 * 知识点：经典排序算法、双重循环、相邻元素比较交换
 */
#include <stdio.h>

int main()
{
    int a[10];
    int i, j, t;
    
    printf("请输入10个整数：\n");
    for (i = 0; i < 10; i++) {
        scanf("%d", &a[i]);
    }
    
    /* 冒泡排序 */
    for (i = 0; i < 9; i++) {           /* 共进行9轮比较 */
        for (j = 0; j < 9 - i; j++) {   /* 每轮比较次数递减 */
            if (a[j] > a[j + 1]) {      /* 相邻元素比较，逆序则交换 */
                t = a[j];
                a[j] = a[j + 1];
                a[j + 1] = t;
            }
        }
    }
    
    printf("排序后结果：\n");
    for (i = 0; i < 10; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");
    return 0;
}