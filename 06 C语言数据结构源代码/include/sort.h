/**
 * 十大经典排序算法
 * 插入类：直接插入、折半插入、希尔
 * 交换类：冒泡、快速排序
 * 选择类：简单选择、堆排序
 * 归并类：二路归并
 * 非比较：基数排序
 */
#ifndef SORT_H
#define SORT_H

#include "common.h"

/* 所有排序算法操作arr[1..n]（1-based，符合教材经典写法） */

void InsertSort(ElemType arr[], int n);     /* 直接插入 */
void BinInsertSort(ElemType arr[], int n);  /* 折半插入 */
void ShellSort(ElemType arr[], int n);      /* 希尔排序 */

void BubbleSort(ElemType arr[], int n);     /* 冒泡（含提前结束优化） */
void QuickSort(ElemType arr[], int n);      /* 快速排序 */

void SelectSort(ElemType arr[], int n);     /* 简单选择 */
void HeapSort(ElemType arr[], int n);       /* 堆排序（大根堆升序） */

void MergeSort(ElemType arr[], int n);      /* 二路归并排序（递归） */
void RadixSort(int arr[], int n);           /* 基数排序LSD（针对非负整数） */

/* 验证数组是否升序 */
bool IsSorted(ElemType arr[], int n);
/* 拷贝数组 */
void ArrayCopy(ElemType dst[], ElemType src[], int n);
/* 性能测试：对同一随机数组，各算法计时比较 */
void BenchmarkAllSorts(int size);

#endif /* SORT_H */
