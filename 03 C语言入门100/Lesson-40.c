#include <stdio.h>
#include <math.h>
/*
* C 语言实例 - 八进制与二进制相互转换
* https://www.runoob.com/cprogramming/c-examples.html
*/

int convertBinarytoOctal(long long binaryNumber);
int main()
{
    long long binaryNumber;

    printf("输入一个二进制数: ");
    scanf("%lld", &binaryNumber);

    printf("二进制数 %lld 转换为八进制为 %d", binaryNumber, convertBinarytoOctal(binaryNumber));

    return 0;
}

int convertBinarytoOctal(long long binaryNumber)
{
    int octalNumber = 0, decimalNumber = 0, i = 0;

    while(binaryNumber != 0)
    {
        decimalNumber += (binaryNumber%10) * pow(2,i);
        ++i;
        binaryNumber/=10;
    }

    i = 1;

    while (decimalNumber != 0)
    {
        octalNumber += (decimalNumber % 8) * i;
        decimalNumber /= 8;
        i *= 10;
    }

    return octalNumber;
}


long long convertOctalToBinary(int octalNumber);
int main()
{
    int octalNumber;

    printf("输入一个八进制数: ");
    scanf("%d", &octalNumber);

    printf("八进制数 %d 转二进制为 %lld", octalNumber, convertOctalToBinary(octalNumber));

    return 0;
}

long long convertOctalToBinary(int octalNumber)
{
    int decimalNumber = 0, i = 0;
    long long binaryNumber = 0;

    while(octalNumber != 0)
    {
        decimalNumber += (octalNumber%10) * pow(8,i);
        ++i;
        octalNumber/=10;
    }

    i = 1;

    while (decimalNumber != 0)
    {
        binaryNumber += (decimalNumber % 2) * i;
        decimalNumber /= 2;
        i *= 10;
    }

    return binaryNumber;
}

//分别定义函数实现八进制转二进制


int octTodeci(int num);//声明八进制转十进制函数
long long deciTobina(int num);//声明十进制转二进制函数
int powNum(int base,int exp);//声明求base的exp次方函数

int main()
{
    int octNum, deciNum;
    long long binaNum;
    printf("Please enter an octal number:");
    scanf("%d", &octNum);
    deciNum = octTodeci(octNum); 
    binaNum = deciTobina(deciNum);
    printf("The conversion from octal number %d to binary number is %lld.\n%", octNum, binaNum);
}

int octTodeci(int num)//定义八进制转十进制函数
{
    int n = 0,remainder,result = 0;
    while (num != 0)
    {
        remainder = num %10;
        result += remainder*powNum(8, n);
        num /= 10;
        ++n;
    }
    return result;
}

long long  deciTobina(int num)//定义十进制转二进制函数
{
    int remainder,n=0;
    long long result = 0;
    while (num != 0)
    {
        remainder = num % 2;
        result += remainder*powNum(10, n);
        num /= 2;
        ++n;
    }
    return result;
}

int powNum(int base,int exp)//定义求base的exp次方函数
{
    int result = 1,k;
    for (k = exp; k >= 1;--k)
        result *= base;
    return result;
}


int aaaa(int num1, int num7);//声明转十进制函数
int ssss(int num2, int num6);//声明十进制转所求进制函数
int main() 
{
    int num1, num6;
    int num7;
    int num8;
    printf("请输入要转变的数字是几进位制的\n");
    scanf_s("%d", &num7);
    printf("请输入%d进制数和要转变的进制数，中间用,隔开\n", num7);
    scanf_s("%d,%d", &num1, &num6);
    num8 = aaaa(num1, num7);
    ssss(num8, num6);
    return 0;
}
//定义八进制转十进制函数
int aaaa(int num1, int num7) {
    int i = 0;
    int k = 0;
    int  num2 = 0, num3;
    while (num1 != 0) {
        if (num1 % 10 >= num7) {
            k++;
            printf("输入的%d进制数错误\n", num7);
            break;
        }
        num3 = num1 % 10;
        num2 = num2 + (int)pow(num7, i) * num3;
        num1 = num1 / 10;
        i++;
    }
    if (k == 0) {
        return num2;
    }
    return 0;

}
//定义十进制转二进制函数
int ssss(int num2, int num6) {
    int num4, num5 = 0;
    int j = 0;
    while (num2 != 0) {
        num4 = num2 % num6;
        num5 = num5 + (int)pow(10, j) * num4;
        num2 = num2 / num6;
        j++;
    }
    printf("转为%d进制数为%d\n", num6, num5);
    return 0;
}