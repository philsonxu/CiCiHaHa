/*
 * 例5.12 斐波那契（Fibonacci）数列
 * 数列：1,1,2,3,5,8,13,... 前两项为1，后面每项等于前两项之和
 * 知识点：递推/迭代算法、变量轮换
 */
#include <stdio.h>

int main()
{
    int f1 = 1, f2 = 1, f3;
    int i;
    printf("斐波那契数列前20项：\n");
    printf("%-8d%-8d", f1, f2);
    
    for (i = 3; i <= 20; i++) {
        f3 = f1 + f2;
        printf("%-8d", f3);
        if (i % 5 == 0)
            printf("\n");  /* 每行5个 */
        f1 = f2;
        f2 = f3;
    }
    printf("\n");
    return 0;
}