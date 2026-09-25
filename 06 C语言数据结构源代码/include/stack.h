/**
 * 顺序栈（动态数组实现）
 * 特性：后进先出LIFO，push/pop均为O(1)，支持动态扩容
 */
#ifndef STACK_H
#define STACK_H

#include "common.h"

#define STACK_INIT_SIZE 10
#define STACK_INCREMENT 5

typedef struct {
    ElemType *data;
    int top;        /* 栈顶下标，-1表示空栈 */
    int capacity;
} SeqStack;

Status Stack_Init(SeqStack *S);
Status Stack_Destroy(SeqStack *S);
void   Stack_Clear(SeqStack *S);
bool   Stack_Empty(const SeqStack *S);
int    Stack_Size(const SeqStack *S);
Status Stack_Push(SeqStack *S, ElemType e);
Status Stack_Pop(SeqStack *S, ElemType *e);
Status Stack_Top(const SeqStack *S, ElemType *e);
/* 遍历：栈底到栈顶 */
Status Stack_Traverse(const SeqStack *S, VisitFunc visit);

/* ========== 栈的经典应用 ========== */
/* 括号匹配检测：( ) [ ] { }，匹配返回true */
bool BracketMatch(const char *expr);
/* 中缀表达式转后缀（逆波兰式），输入字符串，输出结果到postfix */
int  InfixToPostfix(const char *infix, char postfix[], int maxlen);
/* 计算后缀表达式的值 */
int  EvalPostfix(const char *postfix);

#endif /* STACK_H */
