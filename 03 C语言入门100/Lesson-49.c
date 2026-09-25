#include <stdio.h>
#include <math.h>
/*
* C 语言实例 - 计算标准偏差
* https://www.runoob.com/cprogramming/c-examples.html
*/

float calculateSD(float data[]);

int main()
{
    int i;
    float data[10];

    printf("输入10个元素: ");
    for (i = 0; i < 10; ++i)
        scanf("%f", &data[i]);

    printf("\n标准偏差 = %.6f", calculateSD(data));

    return 0;
}

float calculateSD(float data[])
{
    float sum = 0.0, mean, standardDeviation = 0.0;

    int i;

    for (i = 0; i < 10; ++i)
    {
        sum += data[i];
    }

    mean = sum / 10;

    for (i = 0; i < 10; ++i)
        standardDeviation += pow(data[i] - mean, 2);

    return sqrt(standardDeviation / 10);
}