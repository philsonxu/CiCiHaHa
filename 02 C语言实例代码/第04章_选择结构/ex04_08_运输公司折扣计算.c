/*
 * 例4.8 运输公司运费折扣计算（经典switch例题）
 * 知识点：switch区间处理、整数除法映射、break的使用
 * s<250      无折扣
 * 250<=s<500 2%折扣
 * 500<=s<1000 5%折扣
 * 1000<=s<2000 8%折扣
 * 2000<=s<3000 10%折扣
 * s>=3000    15%折扣
 */
#include <stdio.h>

int main()
{
    int c, s;
    float p, w, d, f;
    printf("请输入单价 重量 距离：");
    scanf("%f,%f,%d", &p, &w, &s);
    
    if (s >= 3000)
        c = 12;
    else
        c = s / 250;
    
    switch (c) {
        case 0:  d = 0;    break;
        case 1:  d = 2;    break;
        case 2:
        case 3:  d = 5;    break;
        case 4:
        case 5:
        case 6:
        case 7:  d = 8;    break;
        case 8:
        case 9:
        case 10:
        case 11: d = 10;   break;
        case 12: d = 15;   break;
    }
    
    f = p * w * s * (1 - d / 100);
    printf("运费=%.2f\n", f);
    return 0;
}