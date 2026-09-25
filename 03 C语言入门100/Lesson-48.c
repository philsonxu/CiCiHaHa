#include <stdio.h>

/*
* C 语言实例 - 数组拷贝
* https://www.runoob.com/cprogramming/c-examples.html
*/
int main()
{
    int original[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 };
    int copied[10];
    int loop;

    for (loop = 0; loop < 10; loop++) {
        copied[loop] = original[loop];
    }
    printf("元素数组 -> 拷贝后的数组 \n");

    for (loop = 0; loop < 10; loop++) {
        printf("   %2d        %2d\n", original[loop], copied[loop]);
    }

    return 0;
}

int ArrayCopy(char* ori, char* cop, char Length)
{
    char loop;
    for (loop = 0; loop < Length; loop++)
    {
        *cop++ = *ori++;
    }
    return 0;
}

int main()
{
    char original[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 };
    char* copiedOne = original;
    char copiedTwo[10];
    char loop;
    char Length;

    Length = sizeof(original);
    printf("元素数组 -> 拷贝后的数组 \n");
    for (loop = 0; loop < sizeof(original); loop++)
    {
        printf("   %2d        %2d\n", original[loop], copiedOne[loop]);
    }
    ArrayCopy(original, copiedTwo, Length);

    copiedTwo[9] = 20;
    printf("值传递：\n");
    printf("值传递更改数据后，只有拷贝的数组中的元素被更改： \n");
    printf("元素数组 -> 拷贝后的数组 \n");
    for (loop = 0; loop < sizeof(original); loop++)
    {
        printf("   %2d        %2d\n", original[loop], copiedTwo[loop]);
    }

    copiedOne[9] = 50;
    printf("地址传递：\n");
    printf("地址传递更改数据后，原数组元素与拷贝的数组中的元素同时被更改： \n");
    printf("元素数组 -> 拷贝后的数组 \n");
    for (loop = 0; loop < sizeof(original); loop++)
    {
        printf("   %2d        %2d\n", original[loop], copiedOne[loop]);
    }

    return 0;
}

int main()
{
    int array[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    //令指针指向一块新内存区，该内存区大小刚好可以容纳旧数组所有元素
    int* arrayPointer = (int*)calloc(sizeof(array) / sizeof(array[0]), sizeof(int)), i;
    for (i = 0; i < sizeof(array) / sizeof(array[0]); i++)
        *arrayPointer++ = array[i];

    *arrayPointer = '\0';//为新数组手动添上结束标记
    arrayPointer -= sizeof(array) / sizeof(array[0]);//指针归首位
    printf("元素数组 -> 拷贝后的数组 \n");
    for (i = 0; i < sizeof(array) / sizeof(array[0]); i++)
        printf("%5d%13d\n", array[i], arrayPointer[i]);
    return 0;
}
