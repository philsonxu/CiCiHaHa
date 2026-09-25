/*
 * 例4.5 求一元二次方程ax²+bx+c=0的根
 * 知识点：多分支判断、浮点数比较、数学函数sqrt、复数根
 */
#include <stdio.h>
#include <math.h>

int main()
{
    float a, b, c, disc, x1, x2, realpart, imagpart;
    printf("请输入a,b,c的值：");
    scanf("%f,%f,%f", &a, &b, &c);
    
    if (fabs(a) <= 1e-6) {
        printf("这不是一元二次方程\n");
    } else {
        disc = b * b - 4 * a * c;
        if (fabs(disc) <= 1e-6) {
            /* 两个相等实根 */
            x1 = x2 = -b / (2 * a);
            printf("有两个相等的实根：x1=x2=%.2f\n", x1);
        } else if (disc > 1e-6) {
            /* 两个不等实根 */
            x1 = (-b + sqrt(disc)) / (2 * a);
            x2 = (-b - sqrt(disc)) / (2 * a);
            printf("有两个不等的实根：x1=%.2f, x2=%.2f\n", x1, x2);
        } else {
            /* 共轭复根 */
            realpart = -b / (2 * a);
            imagpart = sqrt(-disc) / (2 * a);
            printf("有两个共轭复根：\n");
            printf("x1=%.2f+%.2fi\n", realpart, imagpart);
            printf("x2=%.2f-%.2fi\n", realpart, imagpart);
        }
    }
    return 0;
}