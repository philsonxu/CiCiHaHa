/*
 * 例7.7 局部变量和全局变量
 * 知识点：作用域、局部变量（函数/块内有效）、全局变量（从定义点到文件尾有效）
 */
#include <stdio.h>

int global = 100;  /* 全局变量 */

void func()
{
    int local_in_func = 200;  /* 函数内局部变量 */
    printf("func中：global=%d, local_in_func=%d\n", global, local_in_func);
}

int main()
{
    int local_in_main = 300;  /* main中的局部变量 */
    printf("main中：global=%d, local_in_main=%d\n", global, local_in_main);
    
    func();
    
    /* 块内局部变量 */
    {
        int block_var = 400;
        printf("块内：block_var=%d\n", block_var);
    }
    /* printf("%d", block_var);  错误：block_var超出作用域 */
    
    return 0;
}