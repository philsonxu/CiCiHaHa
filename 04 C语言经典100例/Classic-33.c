#include <stdio.h>
#include <math.h>

#define MAX 1000  // 最大数组大小

int prime[MAX];  // 存储是否为质数的数组

// 判断数字是否为质数的简单方法（暴力法）
int isPrimeNaive(int n)
{
    if (n <= 1)  // 1及以下的数字不是质数
        return 0;
    for (int i = 2; i < n; i++)  // 从2到n-1逐一检查是否能整除n
        if (n % i == 0)  // 如果能整除，则不是质数
            return 0;
    return 1;  // 找不到因数，则是质数
}

// 优化后的质数判断方法：只检查到sqrt(n)，并跳过偶数
int isPrime(int n)
{
    if (n <= 1)  // 1及以下的数字不是质数
        return 0;
    if (n == 2)  // 2是质数
        return 1;
    if (n % 2 == 0)  // 排除偶数
        return 0;

    int limit = (int)sqrt((double)n);  // 只需要检查到sqrt(n)
    for (int i = 3; i <= limit; i += 2)  // 从3开始，只检查奇数
    {
        if (n % i == 0)  // 如果能整除，则不是质数
            return 0;
    }
    return 1;  // 通过所有测试，n是质数
}

// 筛法初始化质数表
void sieve()
{
    for (int i = 0; i < MAX; i++)
        prime[i] = 1;  // 假设所有数都是质数
    prime[0] = prime[1] = 0;  // 0和1不是质数

    int limit = (int)sqrt((double)MAX);  // 只需要检查到sqrt(MAX)
    for (int i = 2; i <= limit; i++)  // 从2开始，遍历每个数字
    {
        if (prime[i])  // 如果i是质数
        {
            for (int j = i * i; j < MAX; j += i)  // 标记i的倍数为非质数
                prime[j] = 0;
        }
    }
}

// 通过筛法判断一个数是否为质数
int isPrimeSieve(int n)
{
    return prime[n];  // 直接返回该位置是否为质数
}

/*
* C 语言经典100例-33
* https://www.runoob.com/cprogramming/c-exercise-example33.html
* 题目：筛法判断质数。
*/
int main()
{
    sieve();  // 初始化质数筛选

    printf("筛法判断质数(0不是,1是)\n");
    // 测试各种数字是否为质数
    printf("N=%3d %d\n", 1, isPrime(1));  // 输出：1 不是质数
    printf("N=%3d %d\n", 2, isPrime(2));  // 输出：2 是质数
    printf("N=%3d %d\n", 3, isPrime(3));  // 输出：3 是质数
    printf("N=%3d %d\n", 4, isPrime(4));  // 输出：4 不是质数
    printf("N=%3d %d\n", 7, isPrime(7));  // 输出：7 是质数
    printf("N=%3d %d\n", 9, isPrime(9));  // 输出：9 不是质数
    printf("N=%3d %d\n", 13, isPrime(13));  // 输出：13 是质数
    printf("N=%3d %d\n", 17, isPrime(17));  // 输出：17 是质数
    printf("N=%3d %d\n", 100, isPrime(100));  // 输出：100 不是质数
    printf("N=%3d %d\n", 23, isPrime(23));  // 输出：23 是质数
    printf("N=%3d %d\n", 301, isPrime(301));  // 输出：1 不是质数

    return 0;
}