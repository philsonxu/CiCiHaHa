/**
 * 公共头文件：通用类型定义、状态码、工具宏
 */
#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>

/* 通用状态码 */
typedef int Status;
#define OK          1
#define ERROR       0
#define OVERFLOW   -1
#define EMPTY      -2
#define NOT_FOUND  -3

/* 泛型元素类型（可按需修改为自定义结构体） */
typedef int ElemType;

/* 比较函数指针：用于查找、排序 */
typedef int (*CompareFunc)(ElemType a, ElemType b);
/* 打印函数指针：用于遍历输出 */
typedef void (*VisitFunc)(ElemType e);

/* 工具宏 */
#define MAX(a,b)    ((a)>(b)?(a):(b))
#define MIN(a,b)    ((a)<(b)?(a):(b))
#define SWAP(a,b,t) do{t=a;a=b;b=t;}while(0)
#define ARRAY_LEN(arr) (sizeof(arr)/sizeof((arr)[0]))

/* 默认打印：整数 */
static inline void print_int(ElemType e) {
    printf("%d ", e);
}

/* 默认比较：升序 */
static inline int cmp_int(ElemType a, ElemType b) {
    return a - b;
}

#endif /* COMMON_H */
