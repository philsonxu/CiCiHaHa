/*
 * 例4.3 if-else嵌套 - 分段函数计算
 *   y = -1 (x<0)
 *   y = 0  (x=0)
 *   y = 1  (x>0)
 * 知识点：多分支判断、if嵌套、else配对原则（就近原则）
 */
#include <stdio.h>

int main()
{
    int x, y;
    printf("请输入x的值：");
    scanf("%d", &x);
    
    if (x < 0)
        y = -1;
    else if (x == 0)
        y = 0;
    else
        y = 1;
    
    printf("x=%d, y=%d\n", x, y);
    return 0;
}