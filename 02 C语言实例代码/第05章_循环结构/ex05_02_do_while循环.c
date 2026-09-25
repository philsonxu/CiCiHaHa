/*
 * 例5.2 do-while循环 - 求1到100的和
 * 知识点：do-while结构、先执行后判断（循环体至少执行一次）
 * while与do-while的区别：while先判断后执行
 */
#include <stdio.h>

int main()
{
    int i = 1, sum = 0;
    do {
        sum = sum + i;
        i++;
    } while (i <= 100);
    printf("1+2+...+100 = %d\n", sum);
    
    /* 对比：当条件一开始就不成立时 */
    int j = 10;
    printf("\n--- while vs do-while 对比 ---\n");
    while (j < 5) {
        printf("while循环：这行不会打印\n");
        j++;
    }
    
    do {
        printf("do-while循环：这行会打印一次\n");
    } while (j < 5);
    
    return 0;
}