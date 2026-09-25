/*
 * 例6.4 选择排序
 * 知识点：选择排序算法、每轮找最小/最大值交换到对应位置
 */
#include <stdio.h>

int main()
{
    int a[10];
    int i, j, min_idx, t;
    
    printf("请输入10个整数：\n");
    for (i = 0; i < 10; i++) {
        scanf("%d", &a[i]);
    }
    
    /* 选择排序（升序） */
    for (i = 0; i < 9; i++) {
        min_idx = i;  /* 记录最小值下标 */
        for (j = i + 1; j < 10; j++) {
            if (a[j] < a[min_idx]) {
                min_idx = j;
            }
        }
        /* 将最小值交换到第i个位置 */
        if (min_idx != i) {
            t = a[i];
            a[i] = a[min_idx];
            a[min_idx] = t;
        }
    }
    
    printf("排序后结果：\n");
    for (i = 0; i < 10; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");
    return 0;
}