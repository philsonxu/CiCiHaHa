/*
 * 例6.2 求数组中的最大值及其位置
 * 知识点：遍历数组、打擂台算法求极值
 */
#include <stdio.h>

int main()
{
    int a[10];
    int i, max, max_pos;
    
    printf("请输入10个整数：\n");
    for (i = 0; i < 10; i++) {
        scanf("%d", &a[i]);
    }
    
    max = a[0];
    max_pos = 0;
    for (i = 1; i < 10; i++) {
        if (a[i] > max) {
            max = a[i];
            max_pos = i;
        }
    }
    
    printf("最大值为：%d，位置在a[%d]\n", max, max_pos);
    return 0;
}