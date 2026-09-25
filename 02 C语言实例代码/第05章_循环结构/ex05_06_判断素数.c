/*
 * 例5.6 判断素数（质数）
 * 知识点：素数判断算法、break提前退出、标志变量法
 */
#include <stdio.h>
#include <math.h>

int main()
{
    int n, i;
    int is_prime = 1;  /* 标志变量，1表示是素数 */
    
    printf("请输入一个正整数：");
    scanf("%d", &n);
    
    if (n <= 1) {
        is_prime = 0;
    } else {
        for (i = 2; i <= sqrt(n); i++) {
            if (n % i == 0) {
                is_prime = 0;
                break;  /* 找到一个因子，不是素数，跳出循环 */
            }
        }
    }
    
    if (is_prime)
        printf("%d 是素数\n", n);
    else
        printf("%d 不是素数\n", n);
    
    return 0;
}