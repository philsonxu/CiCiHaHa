#include <stdio.h>
#include <math.h>

/*
* C 语言实例 - 判断Armstrong数(阿姆斯壮数)
* https://www.runoob.com/cprogramming/c-examples.html
*/
int main()
{
    int number, originalNumber, remainder, result = 0;

    printf("输入三位数: ");
    scanf("%d", &number);

    originalNumber = number;

    while (originalNumber != 0)
    {
        remainder = originalNumber % 10;
        result += remainder * remainder * remainder;
        originalNumber /= 10;
    }

    if (result == number)
        printf("%d 是 Armstrong 数", number);
    else
        printf("%d 不是 Armstrong 数", number);

    return 0;
}

/*======================================================================*/

int main()
{
    int low, high, i, temp1, temp2, remainder, n = 0, result = 0;

    printf("输入两个整数: ");
    scanf("%d %d", &low, &high);
    printf("%d 和 %d 之间的 Armstrong 数为: ", low, high);

    for (i = low + 1; i < high; ++i)
    {
        temp2 = i;
        temp1 = i;

        // 计算
        while (temp1 != 0)
        {
            temp1 /= 10;
            ++n;
        }

        while (temp2 != 0)
        {
            remainder = temp2 % 10;
            result += pow(remainder, n);
            temp2 /= 10;
        }

        if (result == i) {
            printf("%d ", i);
        }

        n = 0;
        result = 0;

    }
    return 0;
}

/*======================================================================*/

int checkPrimeNumber(int n);
int checkArmstrongNumber(int n);
long long power_long(long long x, int y);

int main()
{
    int n, flag;

    printf("输入正整数: ");
    scanf("%d", &n);

    // 检测素数
    flag = checkPrimeNumber(n);
    if (flag == 1)
        printf("%d 是素数。\n", n);
    else
        printf("%d 不是素数\n", n);

    // 检测 Armstrong 数
    flag = checkArmstrongNumber(n);
    if (flag == 1)
        printf("%d 是 Armstrong 数。", n);
    else
        printf("%d 不是 Armstrong 数。", n);
    return 0;
}

/*
int checkPrimeNumber(int n)
{
    int i, flag = 1;

    for (i = 2; i <= n / 2; ++i)
    {
        // 非素数条件
        if (n % i == 0)
        {
            flag = 0;
            break;
        }
    }
    return flag;
}
*/

int checkArmstrongNumber(int number)
{
    int originalNumber, remainder, result = 0, n = 0, flag;

    originalNumber = number;

    while (originalNumber != 0)
    {
        originalNumber /= 10;
        ++n;
    }

    originalNumber = number;

    while (originalNumber != 0)
    {
        remainder = originalNumber % 10;
        result += pow(remainder, n);
        originalNumber /= 10;
    }

    // 判断条件
    if (result == number)
        flag = 1;
    else
        flag = 0;

    return flag;
}

long long power_long(long long x, int y)
{
    long long sum = 1;
    while (y--)
    {
        sum *= x;
    }
    return sum;
}

int check_arm(long long n)
{
    long long temp, sum = 0, remainder, count = 0;
    temp = n;
    while (temp * 10 / 10 != 0)
    {
        temp /= 10;
        ++count;
    }
    temp = n;
    while (temp * 10 / 10 != 0)
    {
        sum += power_long(temp % 10, count);
        temp /= 10;
    }
    if (n == sum)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/*======================================================================*/

int main()
{
    long long temp, n, result, a, b, i;
    int flag;
    printf("Please enter a number:\n");
    scanf("%lld", &n);
    result = check_arm(n);
    if (result == 1)
    {
        printf("Yes, it's an Armstrong number.\n\n");
    }
    else
    {
        printf("No, it isn't an Armstrong number.\n\n");
    }
    printf("Please enter two numbers as an interregional,seperated by a space.\n");
    scanf("%lld %lld", &a, &b);
    flag = 1;
    if (a == b)
    {
        printf("That is no an valid interregional.\n");
        flag = 0;
    }
    else if (a > b)
    {
        temp = a;
        a = b;
        b = temp;
    }
    if (flag == 1)
    {
        printf("Armstrong number(s) between %lld and %lld is(are):\n", a, b);
        for (i = a; i <= b; i++)
        {
            if (check_arm(i) == 1)
            {
                printf("%lld ", i);
            }
        }
    }
    return 0;
}

/*======================================================================*/

int main()
{
    int low, high;
    scanf("%d %d", &low, &high);
    while (low <= high) {
        if (arm(low) == 1)
            printf("%d \t", low);
        low++;
    }
}

int arm(int a)
{
    int b = 0, c = 0, e = 0; //初始化 
    c = a;
    while (c > 0) {
        if (c % 10 >= 0) e++;
        c = c / 10;
    }
    c = a;
    while (a > 0) {
        b = b + pow(a % 10, e);
        a = a / 10;
    }
    if (c == b) return 1;
    else return 0;
}
