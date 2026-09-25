#include <stdio.h>
#include <math.h>

/*
* C 语言实例 - 二进制与十进制相互转换
* https://www.runoob.com/cprogramming/c-examples.html
*/
int convertBinaryToDecimal(long long n);

int main()
{
    long long n;
    printf("输入一个二进制数: ");
    scanf("%lld", &n);
    printf("二进制数 %lld 转换为十进制为 %d", n, convertBinaryToDecimal(n));
    return 0;
}

int convertBinaryToDecimal(long long n)
{
    int decimalNumber = 0, i = 0, remainder;
    while (n != 0)
    {
        remainder = n % 10;
        n /= 10;
        decimalNumber += remainder * pow(2, i);
        ++i;
    }
    return decimalNumber;
}


long long convertDecimalToBinary(int n);

int main()
{
    int n;
    printf("输入一个十进制数: ");
    scanf("%d", &n);
    printf("十进制数 %d 转换为二进制位 %lld", n, convertDecimalToBinary(n));
    return 0;
}

long long convertDecimalToBinary(int n)
{
    long long binaryNumber = 0;
    int remainder, i = 1, step = 1;

    while (n != 0)
    {
        remainder = n % 2;
        printf("Step %d: %d/2, 余数 = %d, 商 = %d\n", step++, n, remainder, n / 2);
        n /= 2;
        binaryNumber += remainder * i;
        i *= 10;
    }
    return binaryNumber;
}

//求a的b次方函数
unsigned ppow(unsigned a, unsigned b)
{
    unsigned cc = 1;
    while (b) {
        cc *= a;
        b--;
    }
    return cc;
}

int main()
{
    unsigned long long n = 0;
    unsigned  i, k = 0, count = 0;
    printf("请输入2进制数：");

    scanf_s("%lld", &n);          //用scanf（）在Visual Studio 2019 会警告返回值被忽略
    while (n) {
        i = n % 10;                //这个说错了，2和10都可以，用10会比较好
        if (i) {                  //判断i是否为0
            count += ppow(2, k);
        }
        k++;                      //k做为2的次方数，每次n /= 10 就+1
        n /= 10;
    }
    printf(" 转成10进制= %d", count);
    return 0;
}