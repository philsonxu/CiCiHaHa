#include <stdio.h>

/*
* C 语言实例 - 查找数组中最大的元素值
* https://www.runoob.com/cprogramming/c-examples.html
*/
int main()
{
    int array[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 };
    int loop, largest;

    largest = array[0];

    for (loop = 1; loop < 10; loop++) {
        if (largest < array[loop])
            largest = array[loop];
    }

    printf("最大元素为 %d", largest);

    return 0;
}

int main()
{
    int i, n;
    float arr[100];

    printf("输入元素个数（0~100）: ");
    scanf("%d", &n);
    printf("\n");

    // 接收用户输入
    for (i = 0; i < n; ++i)
    {
        printf("输入数字 %d: ", i + 1);
        scanf("%f", &arr[i]);
    }

    // 循环，并将最大元素存储到 arr[0]
    for (i = 1; i < n; ++i)
    {
        // 如果要查找最小值可以将 < 换成 > 
        if (arr[0] < arr[i])
            arr[0] = arr[i];
    }
    printf("最大元素为 = %.2f", arr[0]);

    return 0;
}

int main()
{
    int n[] = { 3,2,1,4,7,6,5,8,0,9 };
    int largest = 0, i, num;
    num = sizeof(n) / sizeof(n[0]);
    for (i = 0; i < num; i++)
        largest = (n[i] > largest) ? n[i] : largest;
    printf("Largest Number is:%d", largest);
}