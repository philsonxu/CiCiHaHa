#include <stdio.h>
#include <stdlib.h>

void print_array(int array[], int n);
void move_array(int array[], int n, int offset);

// 通过 shiftArray 函数来实现数组元素的移动。
void shift_array(int arr[], int n, int m) 
{
    //int temp[m];
    int* temp;
    temp = (int *)malloc(m* sizeof(int));

    // 保存最后 m 个数到临时数组
    for (int i = n - m, j = 0; i < n; i++, j++) {
        temp[j] = arr[i];
    }

    // 将前面的 n-m 个数向后移动 m 个位置
    for (int i = n - m - 1; i >= 0; i--) {
        arr[i + m] = arr[i];
    }

    // 将临时数组中的数放到最前面
    for (int i = 0; i < m; i++) {
        arr[i] = temp[i];
    }
}


//打印数组
void print_array(int array[], int n)
{
    int i;
    for (i = 0; i < n; ++i)
        printf("%4d", array[i]);
    printf("\n");
}

//滚动数组
void move_array(int array[], int n, int offset)
{
    int* p, * arr_end;
    arr_end = array + n;      //数组最后一个元素的下一个位置
    int last;

    //滚动直到偏移量为0
    while (offset)
    {
        last = *(arr_end - 1);
        for (p = arr_end - 1; p != array; --p)   //向右滚动一位
            *p = *(p - 1);
        *array = last;
        --offset;
    }
}

// 在 main 函数中获取用户输入的数组和要移动的位置，调用s hiftArray 函数，最后输出移动后的数组

/*
* C 语言经典100例-68
* https://www.runoob.com/cprogramming/c-exercise-example68.html
* 题目：数组移动问题
* 有 n 个整数，使其前面各数顺序向后移 m 个位置，最后m个数变成最前面的 m 个数。
* 程序分析：可以使用一个辅助数组来保存需要移动的元素，然后再将元素按照要求移动到正确的位置。
*/
int main()
{
    int n, m;

    printf("数组移动问题\n");
    printf("请输入整数个数 n：");
    scanf("%d", &n);

    printf("请输入向后移动的位置 m：");
    scanf("%d", &m);

    //int arr[n];
    int* arr;
    arr = (int*)malloc(n * sizeof(int));

    printf("请输入 %d 个整数：", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    shift_array(arr, n, m);

    printf("移动后的数组：");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    return 0;
}

/*=============================================================*/

int main()
{
    int arr[20];
    int i, n, offset;
    //输入数组大小和数组内容
    printf("Total numbers?\n");
    scanf("%d", &n);
    printf("Input %d numbers.\n", n);
    for (i = 0; i < n; i++)
        scanf("%d", &arr[i]);
    //输入滚动偏移量
    printf("Set your offset.\n");
    scanf("%d", &offset);
    printf("Offset is %d.\n", offset);
    //打印滚动前数组
    print_array(arr, n);
    //滚动数组并打印
    move_array(arr, n, offset);
    print_array(arr, n);
}