/*
 * 例7.5 递归经典问题 - 汉诺塔(Hanoi)
 * 知识点：复杂递归分解、问题规模减小策略
 */
#include <stdio.h>

/* 将n个盘子从A座借助C座移到B座 */
void hanoi(int n, char A, char B, char C)
{
    if (n == 1) {
        printf("将第1个盘子从%c --> %c\n", A, B);
    } else {
        hanoi(n - 1, A, C, B);     /* 将n-1个从A移到C（借助B） */
        printf("将第%d个盘子从%c --> %c\n", n, A, B);  /* 把最底下的移到B */
        hanoi(n - 1, C, B, A);     /* 将n-1个从C移到B（借助A） */
    }
}

int main()
{
    int n;
    printf("请输入盘子个数：");
    scanf("%d", &n);
    printf("移动步骤：\n");
    hanoi(n, 'A', 'B', 'C');
    return 0;
}