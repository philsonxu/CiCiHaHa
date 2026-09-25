/**
 * ============================================================================
 * C语言指针高级用法示例
 * 主题：二级指针、函数指针、void指针、指针运算、数组指针、指针数组
 * 编译：gcc -Wall -std=c99 -o pointer_advanced pointer_advanced.c
 * ============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ==========================================================================
 * 1. 二级指针（指向指针的指针）：常用于函数内分配内存、修改外部指针
 * ========================================================================== */
// 通过二级指针在函数内部为外部指针分配内存
int allocate_memory(char **ptr, size_t size) {
    *ptr = (char *)malloc(size);
    if (*ptr == NULL) {
        return -1;
    }
    memset(*ptr, 0, size);
    return 0;
}

// 二维数组的二级指针访问
void two_dimensional_array_demo() {
    printf("\n===== 1.1 二级指针与二维数组 =====\n");
    int rows = 3, cols = 4;

    // 分配二级指针（行指针数组）
    int **matrix = (int **)malloc(rows * sizeof(int *));
    for (int i = 0; i < rows; i++) {
        matrix[i] = (int *)malloc(cols * sizeof(int));
    }

    // 赋值
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = i * cols + j + 1;
        }
    }

    // 遍历（三种等价方式）
    printf("使用 matrix[i][j] 访问：\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%2d ", matrix[i][j]);
        }
        printf("\n");
    }

    // 使用指针偏移访问
    printf("使用指针偏移访问：\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%2d ", *(*(matrix + i) + j));
        }
        printf("\n");
    }

    // 释放内存（先释放每行，再释放行指针数组）
    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

/* ==========================================================================
 * 2. 函数指针：实现回调、策略模式、函数表
 * ========================================================================== */
// 定义运算函数类型
typedef int (*MathOperation)(int, int);

int add(int a, int b)      { return a + b; }
int subtract(int a, int b) { return a - b; }
int multiply(int a, int b) { return a * b; }
int divide(int a, int b)   { return b != 0 ? a / b : 0; }

// 使用函数指针作为参数（回调函数模式）
int calculate(int a, int b, MathOperation op) {
    return op(a, b);
}

// 函数表（命令分发模式）
void function_table_demo() {
    printf("\n===== 2.1 函数指针与函数表 =====\n");
    MathOperation operations[] = {add, subtract, multiply, divide};
    const char *op_names[] = {"加法", "减法", "乘法", "除法"};
    int a = 20, b = 5;

    for (int i = 0; i < 4; i++) {
        printf("%s: %d %s %d = %d\n",
               op_names[i], a,
               i == 0 ? "+" : i == 1 ? "-" : i == 2 ? "*" : "/",
               b, calculate(a, b, operations[i]));
    }
}

/* ==========================================================================
 * 3. void* 泛型指针：实现通用数据结构
 * ========================================================================== */
// 通用打印函数，通过函数指针实现类型定制
typedef void (*PrintFunc)(const void *);

void print_int(const void *data) {
    printf("%d", *(const int *)data);
}

void print_double(const void *data) {
    printf("%.2f", *(const double *)data);
}

void print_string(const void *data) {
    printf("\"%s\"", *(const char *const *)data);
}

// 通用数组打印
void generic_array_print(const void *arr, size_t elem_size, size_t len, PrintFunc print_fn) {
    printf("[ ");
    for (size_t i = 0; i < len; i++) {
        // void* 不能直接偏移，必须先转 char* 做字节级偏移
        const void *elem = (const char *)arr + i * elem_size;
        print_fn(elem);
        if (i < len - 1) printf(", ");
    }
    printf(" ]\n");
}

void void_pointer_demo() {
    printf("\n===== 3. void* 泛型指针 =====\n");

    int int_arr[] = {1, 2, 3, 4, 5};
    double dbl_arr[] = {1.1, 2.2, 3.3, 4.4};
    const char *str_arr[] = {"hello", "world", "c"};

    printf("整数数组：");
    generic_array_print(int_arr, sizeof(int), 5, print_int);

    printf("浮点数数组：");
    generic_array_print(dbl_arr, sizeof(double), 4, print_double);

    printf("字符串数组：");
    generic_array_print(str_arr, sizeof(char *), 3, print_string);
}

/* ==========================================================================
 * 4. 数组指针 vs 指针数组
 * ========================================================================== */
void array_pointer_vs_pointer_array() {
    printf("\n===== 4. 数组指针与指针数组 =====\n");

    // 指针数组：数组里每个元素是指针（可指向不同长度的数组）
    int a = 10, b = 20, c = 30;
    int *ptr_arr[] = {&a, &b, &c};
    printf("指针数组（每个元素是int*）：\n");
    for (int i = 0; i < 3; i++) {
        printf("  ptr_arr[%d] = %d\n", i, *ptr_arr[i]);
    }

    // 数组指针：一个指针，指向一个固定长度的数组
    int arr[3][4] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12}
    };
    int (*arr_ptr)[4] = arr; // arr_ptr 指向含4个int的数组
    printf("\n数组指针（指向长度为4的int数组）：\n");
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            printf("%3d", arr_ptr[i][j]);
        }
        printf("\n");
    }
}

/* ==========================================================================
 * 5. const 与指针的四种组合
 * ========================================================================== */
void const_pointer_demo() {
    printf("\n===== 5. const 与指针 =====\n");
    int value = 100;
    int other = 200;

    // 5.1 指向const数据的指针：不能通过指针修改数据，但指针可以指向别处
    const int *p1 = &value;
    // *p1 = 200;  // 错误！不能修改指向的数据
    p1 = &other;    // 正确：可以改变指针指向
    printf("指向const数据的指针: *p1 = %d\n", *p1);

    // 5.2 const指针：指针本身不能改变指向，但可以修改数据
    int * const p2 = &value;
    *p2 = 150;      // 正确：可以修改数据
    // p2 = &other; // 错误！不能改变指针指向
    printf("const指针: *p2 = %d\n", *p2);

    // 5.3 指向const数据的const指针：都不能改
    const int * const p3 = &value;
    // *p3 = 300;  // 错误
    // p3 = &other;// 错误
    printf("指向const的const指针: *p3 = %d\n", *p3);

    int val = 42;
    int *p4 = &val;
    int **pp = &p4;
    printf("二级指针示例：**pp = %d\n", **pp);
}

/* ==========================================================================
 * 主函数
 * ========================================================================== */
int main() {
    printf("***************************************\n");
    printf("*   C语言指针高级用法示例             *\n");
    printf("***************************************\n");

    // 二级指针分配内存
    char *buf = NULL;
    if (allocate_memory(&buf, 64) == 0) {
        strcpy(buf, "Hello, C Advanced Programming!");
        printf("\n===== 1. 二级指针分配内存 =====\n");
        printf("buf 地址: %p, 内容: %s\n", (void*)buf, buf);
        free(buf);
        buf = NULL;
    }

    two_dimensional_array_demo();
    function_table_demo();
    void_pointer_demo();
    array_pointer_vs_pointer_array();
    const_pointer_demo();

    printf("\n指针高级示例执行完毕！\n");
    return 0;
}
