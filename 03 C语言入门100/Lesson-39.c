#include <stdio.h>
#include <math.h>
/*
* C 语言实例 - 八进制与十进制相互转换
* https://www.runoob.com/cprogramming/c-examples.html
*/

int convertDecimalToOctal(int decimalNumber);

int main()
{
    int decimalNumber;

    printf("输入一个十进制数: ");
    scanf("%d", &decimalNumber);

    printf("十进制数 %d 转换为八进制为 %d", decimalNumber, convertDecimalToOctal(decimalNumber));

    return 0;
}

int convertDecimalToOctal(int decimalNumber)
{
    int octalNumber = 0, i = 1;

    while (decimalNumber != 0)
    {
        octalNumber += (decimalNumber % 8) * i;
        decimalNumber /= 8;
        i *= 10;
    }

    return octalNumber;
}


long long convertOctalToDecimal(int octalNumber);

int main()
{
    int octalNumber;

    printf("输入一个八进制数: ");
    scanf("%d", &octalNumber);

    printf("八进制数 %d  转换为十进制为 %lld", octalNumber, convertOctalToDecimal(octalNumber));

    return 0;
}

long long convertOctalToDecimal(int octalNumber)
{
    int decimalNumber = 0, i = 0;

    while(octalNumber != 0)
    {
        decimalNumber += (octalNumber%10) * pow(8,i);
        ++i;
        octalNumber/=10;
    }

    i = 1;

    return decimalNumber;
}

/*
任意进制转换
srBaseNum: 原进制下的数
srBse: 原进制的基
destBase: 目标进制的基
destBaseNum：转换后的数 
*/ 

int numBaseConversion(int srBaseNum, int srBase, int destBase)
{
    int destBaseNum=0, i=0;
    while (srBaseNum != 0){
        destBaseNum += (srBaseNum % destBase) * pow(srBase,i);
        srBaseNum /= destBase;
        i ++;
    }
    return destBaseNum;
}