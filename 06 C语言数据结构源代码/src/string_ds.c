#include "string_ds.h"

Status String_Init(HString *S, const char *chars) {
    if (!S || !chars) return ERROR;
    int len = strlen(chars);
    S->ch = (char*)malloc((len + 2) * sizeof(char)); /* +2: 1-indexed + '\0' */
    if (!S->ch) return OVERFLOW;
    strcpy(S->ch + 1, chars);   /* ch[0]未使用，字符从ch[1]开始 */
    S->length = len;
    return OK;
}

Status String_Destroy(HString *S) {
    if (!S || !S->ch) return ERROR;
    free(S->ch);
    S->ch = NULL;
    S->length = 0;
    return OK;
}

int String_Length(const HString *S) { return S ? S->length : 0; }

Status String_Concat(HString *T, const HString *S1, const HString *S2) {
    if (!T || !S1 || !S2) return ERROR;
    if (T->ch) free(T->ch);
    int len = S1->length + S2->length;
    T->ch = (char*)malloc(len + 2);
    if (!T->ch) return OVERFLOW;
    for (int i = 1; i <= S1->length; i++) T->ch[i] = S1->ch[i];
    for (int i = 1; i <= S2->length; i++) T->ch[S1->length+i] = S2->ch[i];
    T->ch[len+1] = '\0';
    T->length = len;
    return OK;
}

Status String_SubString(HString *Sub, const HString *S, int pos, int len) {
    if (!S || pos < 1 || pos > S->length || len < 0 || pos+len-1 > S->length)
        return ERROR;
    if (Sub->ch) free(Sub->ch);
    Sub->ch = (char*)malloc(len + 2);
    if (!Sub->ch) return OVERFLOW;
    for (int i = 1; i <= len; i++)
        Sub->ch[i] = S->ch[pos + i - 1];
    Sub->ch[len+1] = '\0';
    Sub->length = len;
    return OK;
}

int String_Compare(const HString *S1, const HString *S2) {
    int i;
    for (i = 1; i <= S1->length && i <= S2->length; i++) {
        if (S1->ch[i] != S2->ch[i]) return S1->ch[i] - S2->ch[i];
    }
    return S1->length - S2->length;
}

void String_Print(const HString *S) {
    if (!S || !S->ch) return;
    printf("%s", S->ch + 1);
}

/* ========== BF朴素匹配 ========== */
int BF_Index(const HString *S, const HString *T, int pos) {
    int i = pos, j = 1;
    while (i <= S->length && j <= T->length) {
        if (S->ch[i] == T->ch[j]) { i++; j++; }
        else { i = i - j + 2; j = 1; }  /* 主串回溯到下一个起点 */
    }
    return j > T->length ? i - T->length : 0;
}

/* ========== KMP算法 ========== */
void KMP_GetNext(const HString *T, int next[]) {
    int i = 1, j = 0;
    next[1] = 0;
    while (i < T->length) {
        if (j == 0 || T->ch[i] == T->ch[j]) {
            i++; j++;
            next[i] = j;
        } else {
            j = next[j];
        }
    }
}

void KMP_GetNextVal(const HString *T, int nextval[]) {
    int i = 1, j = 0;
    nextval[1] = 0;
    while (i < T->length) {
        if (j == 0 || T->ch[i] == T->ch[j]) {
            i++; j++;
            if (T->ch[i] != T->ch[j])
                nextval[i] = j;
            else
                nextval[i] = nextval[j];
        } else {
            j = nextval[j];
        }
    }
}

int KMP_Index(const HString *S, const HString *T, int pos) {
    int i = pos, j = 1;
    int *next = (int*)malloc((T->length + 2) * sizeof(int));
    KMP_GetNext(T, next);
    while (i <= S->length && j <= T->length) {
        if (j == 0 || S->ch[i] == T->ch[j]) { i++; j++; }
        else { j = next[j]; }  /* 仅模式串回溯，主串指针i不回退 */
    }
    free(next);
    return j > T->length ? i - T->length : 0;
}
