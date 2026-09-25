/*
 * 例5.10 求所有水仙花数
 * 水仙花数：一个3位数，各位数字立方和等于该数本身（如153=1³+5³+3³）
 * 知识点：数位分解、循环遍历
 */
#include <stdio.h>

int main()
{
    int n, i, j, k;
    printf("所有水仙花数：\n");
    
    for (n = 100; n < 1000; n++) {
        i = n / 100;        /* 百位 */
        j = n / 10 % 10;    /* 十位 */
        k = n % 10;         /* 个位 */
        if (n == i*i*i + j*j*j + k*k*k) {
            printf("%d ", n);
        }
    }
    printf("\n");
    return 0;
}