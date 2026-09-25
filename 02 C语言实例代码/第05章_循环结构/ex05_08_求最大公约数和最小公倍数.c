/*
 * 例5.8 求最大公约数和最小公倍数 - 辗转相除法（欧几里得算法）
 * 知识点：while循环、经典算法
 */
#include <stdio.h>

int main()
{
    int m, n, a, b, t;
    printf("请输入两个正整数：");
    scanf("%d,%d", &m, &n);
    
    a = m;
    b = n;
    
    /* 辗转相除法求最大公约数 */
    while (b != 0) {
        t = a % b;
        a = b;
        b = t;
    }
    
    printf("最大公约数：%d\n", a);
    printf("最小公倍数：%d\n", m * n / a);
    return 0;
}