#include <stdio.h>
#include "classic.h"

// 递归函数，用于计算阶乘
unsigned long long factorial_recursion(int n);

/*
* C 语言经典100例-26
* https://www.runoob.com/cprogramming/c-exercise-example26.html
* 题目：利用递归方法阶乘
*/
int main()
{
    // 打印 0 到 5 的阶乘
    for (int num = 0; num < 16; num++) {
        printf("%d! = %lld\n", num, factorial_recursion(num));
    }
    return 0; // 程序正常结束
}

/*
// 计算阶乘的递归函数
// 参数：n - 需要计算阶乘的数
// 返回值：n 的阶乘
unsigned long long factorial(int n) {
    // 定义变量存储结果
    unsigned long long result;

    // 阶乘的递归基准条件
    if (n == 0) {
        result = 1; // 0! = 1
    }
    else {
        result = n * factorial(n - 1); // 递归计算 n! = n * (n-1)!
    }
    return result; // 返回计算结果
}
*/