/*
 * 例5.5 break和continue语句
 * 知识点：break跳出整个循环、continue跳过本次循环剩余语句
 */
#include <stdio.h>

int main()
{
    int i;
    printf("break示例（输出1-10，遇到5停止）：\n");
    for (i = 1; i <= 10; i++) {
        if (i == 5)
            break;  /* 跳出整个for循环 */
        printf("%d ", i);
    }
    printf("\n");
    
    printf("\ncontinue示例（输出1-10，跳过5）：\n");
    for (i = 1; i <= 10; i++) {
        if (i == 5)
            continue;  /* 跳过本次，直接进入下一次迭代 */
        printf("%d ", i);
    }
    printf("\n");
    
    return 0;
}