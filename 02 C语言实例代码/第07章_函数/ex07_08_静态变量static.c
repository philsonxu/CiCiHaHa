/*
 * 例7.8 静态局部变量 static
 * 知识点：static变量在程序运行期间始终存在，只初始化一次
 */
#include <stdio.h>

int func2()
{
    static int count = 0;  /* 静态局部变量，只初始化一次 */
    count++;
    return count;
}

int main()
{
    printf("第1次调用：%d\n", func2());  /* 1 */
    printf("第2次调用：%d\n", func2());  /* 2 */
    printf("第3次调用：%d\n", func2());  /* 3 */
    printf("第4次调用：%d\n", func2());  /* 4 */
    return 0;
}