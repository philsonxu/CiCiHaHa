#include <stdio.h>
/*
// 计算阶乘的函数
unsigned long long factorial_25(int n) {
    unsigned long long result = 1;
    for (int i = 1; i <= n; i++) {
        result *= i;
    }
    return result;
}
*/
unsigned long long factorial(int n);

/*
* C 语言经典100例-25
* https://www.runoob.com/cprogramming/c-exercise-example25.html
* 题目：阶乘之和
* 求 1! + 2! + 3! + ... + 20! 的和。
*/
int main()
{
    unsigned long long sum = 0;
    unsigned long long val = 0;

    // 计算1 + 2! + 3! + ... + 20!
    for (int i = 1; i <= 20; i++) {
        val = factorial(i);
        sum += val;
        printf("%3d!=%llu sum = %llu\n", i, val, sum);
    }

    // 输出结果
    printf("1 + 2! + 3! + ... + 20!的和是: %llu\n", sum);

    return 0;
}