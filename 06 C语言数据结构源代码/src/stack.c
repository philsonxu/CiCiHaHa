#include "stack.h"

Status Stack_Init(SeqStack *S) {
    if (!S) return ERROR;
    S->data = (ElemType*)malloc(STACK_INIT_SIZE * sizeof(ElemType));
    if (!S->data) return OVERFLOW;
    S->top = -1;
    S->capacity = STACK_INIT_SIZE;
    return OK;
}

Status Stack_Destroy(SeqStack *S) {
    if (!S || !S->data) return ERROR;
    free(S->data);
    S->data = NULL;
    S->top = S->capacity = 0;
    return OK;
}

void Stack_Clear(SeqStack *S) {
    if (S) S->top = -1;
}

bool Stack_Empty(const SeqStack *S) {
    return S ? S->top == -1 : true;
}

int Stack_Size(const SeqStack *S) {
    return S ? S->top + 1 : 0;
}

static Status stack_ensure(SeqStack *S) {
    if (S->top + 1 >= S->capacity) {
        int new_cap = S->capacity + STACK_INCREMENT;
        ElemType *p = (ElemType*)realloc(S->data, new_cap * sizeof(ElemType));
        if (!p) return OVERFLOW;
        S->data = p;
        S->capacity = new_cap;
    }
    return OK;
}

Status Stack_Push(SeqStack *S, ElemType e) {
    if (!S) return ERROR;
    if (stack_ensure(S) != OK) return OVERFLOW;
    S->data[++S->top] = e;
    return OK;
}

Status Stack_Pop(SeqStack *S, ElemType *e) {
    if (!S || Stack_Empty(S)) return ERROR;
    if (e) *e = S->data[S->top];
    S->top--;
    return OK;
}

Status Stack_Top(const SeqStack *S, ElemType *e) {
    if (!S || !e || Stack_Empty(S)) return ERROR;
    *e = S->data[S->top];
    return OK;
}

Status Stack_Traverse(const SeqStack *S, VisitFunc visit) {
    if (!S || !visit) return ERROR;
    for (int i = 0; i <= S->top; i++)
        visit(S->data[i]);
    printf("\n");
    return OK;
}

/* ========== 应用：括号匹配 ========== */
bool BracketMatch(const char *expr) {
    /* 简单实现：用一个char栈，左括号压栈，右括号比较栈顶 */
    char stack[256];
    int top = -1;
    for (int i = 0; expr[i]; i++) {
        char c = expr[i];
        if (c == '(' || c == '[' || c == '{') {
            if (top >= 255) return false;
            stack[++top] = c;
        } else if (c == ')' || c == ']' || c == '}') {
            if (top < 0) return false;
            char left = stack[top--];
            if ((c==')' && left!='(') || (c==']' && left!='[') || (c=='}' && left!='{'))
                return false;
        }
    }
    return top == -1;
}

/* ========== 应用：中缀转后缀（支持 + - * / 数字，单字符） ========== */
static int op_priority(char op) {
    switch (op) {
        case '+': case '-': return 1;
        case '*': case '/': return 2;
        case '(': return 0;
        default: return -1;
    }
}

int InfixToPostfix(const char *infix, char postfix[], int maxlen) {
    char opstk[256];
    int top = -1, k = 0;
    for (int i = 0; infix[i]; i++) {
        char c = infix[i];
        if (c >= '0' && c <= '9') {
            if (k >= maxlen-1) return -1;
            postfix[k++] = c;
        } else if (c == '(') {
            opstk[++top] = c;
        } else if (c == ')') {
            while (top >= 0 && opstk[top] != '(') {
                postfix[k++] = opstk[top--];
            }
            top--;  /* 弹出( */
        } else {  /* 运算符 */
            while (top >= 0 && op_priority(opstk[top]) >= op_priority(c)) {
                postfix[k++] = opstk[top--];
            }
            opstk[++top] = c;
        }
        postfix[k] = ' '; if (k<maxlen-1) k++;
    }
    while (top >= 0) { postfix[k++] = opstk[top--]; postfix[k++] = ' '; }
    postfix[k] = '\0';
    return k;
}

int EvalPostfix(const char *postfix) {
    int stk[256], top = -1;
    for (int i = 0; postfix[i]; i++) {
        char c = postfix[i];
        if (c >= '0' && c <= '9') {
            stk[++top] = c - '0';
        } else if (c == '+' || c == '-' || c == '*' || c == '/') {
            int b = stk[top--], a = stk[top--];
            int res;
            switch (c) {
                case '+': res = a+b; break;
                case '-': res = a-b; break;
                case '*': res = a*b; break;
                case '/': res = a/b; break;
            }
            stk[++top] = res;
        }
    }
    return top >= 0 ? stk[top] : 0;
}
