/**
 * 串（堆分配顺序存储）+ KMP模式匹配算法
 */
#ifndef STRING_H
#define STRING_H

#include "common.h"

typedef struct {
    char *ch;       /* 按1-indexed使用，ch[0]可作为优化空间 */
    int length;
} HString;

Status String_Init(HString *S, const char *chars);
Status String_Destroy(HString *S);
int    String_Length(const HString *S);
Status String_Concat(HString *T, const HString *S1, const HString *S2);
Status String_SubString(HString *Sub, const HString *S, int pos, int len);
int    String_Compare(const HString *S1, const HString *S2);
void   String_Print(const HString *S);

/* 朴素模式匹配：返回主串S中第一次出现子串T的位置（1-based），找不到返回0 */
int    BF_Index(const HString *S, const HString *T, int pos);
/* KMP模式匹配：O(n+m)，通过next数组避免主串回溯 */
int    KMP_Index(const HString *S, const HString *T, int pos);
/* 获取KMP的next数组 */
void   KMP_GetNext(const HString *T, int next[]);
/* 优化版nextval数组 */
void   KMP_GetNextVal(const HString *T, int nextval[]);

#endif /* STRING_H */
