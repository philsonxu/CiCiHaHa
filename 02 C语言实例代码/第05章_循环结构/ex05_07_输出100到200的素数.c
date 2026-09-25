/*
 * 例5.7 输出100~200之间的所有素数
 * 知识点：循环嵌套、continue优化、素数筛选
 */
#include <stdio.h>
#include <math.h>

int main()
{
    int n, i, count = 0;
    printf("100~200之间的素数：\n");
    
    for (n = 101; n <= 200; n += 2) {  /* 偶数不可能是素数，跳过 */
        int is_prime = 1;
        for (i = 2; i <= sqrt(n); i++) {
            if (n % i == 0) {
                is_prime = 0;
                break;
            }
        }
        if (is_prime) {
            printf("%d ", n);
            count++;
            if (count % 5 == 0)  /* 每行输出5个 */
                printf("\n");
        }
    }
    printf("\n共有%d个素数\n", count);
    return 0;
}