/*
 * 例11.4 条件编译
 * 知识点：#ifdef #ifndef #if #else #elif #endif
 * 作用：让部分代码在满足条件时才参与编译
 * 应用场景：防止头文件重复包含、调试开关、跨平台编译
 */
#include <stdio.h>

/* 调试开关 */
#define DEBUG 1

#define WINDOWS 1
/* #define LINUX 1 */

int main()
{
/* #ifdef 如果定义了DEBUG就编译 */
#ifdef DEBUG
    printf("调试模式：程序开始运行...\n");
#endif

    int a = 10, b = 20;
    printf("a + b = %d\n", a + b);

/* #ifndef 如果没定义 */
#ifndef RELEASE
    printf("注意：这是开发版本\n");
#endif

/* #if 常量表达式判断 */
#if WINDOWS
    printf("当前平台：Windows\n");
#elif LINUX
    printf("当前平台：Linux\n");
#else
    printf("当前平台：其他\n");
#endif

/* #ifdef 中使用 #else */
#define ENGLISH
#ifdef ENGLISH
    printf("Hello!\n");
#else
    printf("你好！\n");
#endif

/* #undef 取消宏定义 */
#undef DEBUG
#ifdef DEBUG
    printf("这行不会输出（DEBUG已取消）\n");
#endif

    return 0;
}