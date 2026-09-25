#include <stdio.h>
#include <assert.h>
#include "stack.h"

void test_stack() {
    printf("===== 测试3：顺序栈 SeqStack 及应用 =====\n");
    SeqStack S;
    Stack_Init(&S);
    assert(Stack_Empty(&S));

    for (int i = 1; i <= 6; i++)
        Stack_Push(&S, i * 10);
    printf("压入 10~60 后（栈底到栈顶）：");
    Stack_Traverse(&S, print_int);  /* 10 20 30 40 50 60 */
    assert(Stack_Size(&S) == 6);

    ElemType e;
    Stack_Top(&S, &e);
    printf("栈顶元素：%d\n", e);  /* 60 */
    assert(e == 60);

    Stack_Pop(&S, &e);
    printf("弹出一个：%d，弹出后：", e);
    Stack_Traverse(&S, print_int);  /* 10 20 30 40 50 */

    /* 括号匹配测试 */
    const char *tests[] = {
        "a*(b+c)/[d-{e,f}]",   /* 匹配 */
        "((()))",              /* 匹配 */
        "([)]",                /* 不匹配 */
        "(",                   /* 不匹配 */
    };
    printf("括号匹配测试：\n");
    for (int i = 0; i < 4; i++) {
        printf("  \"%s\" → %s\n", tests[i],
               BracketMatch(tests[i]) ? "✅ 匹配" : "❌ 不匹配");
    }
    assert(BracketMatch(tests[0]) == true);
    assert(BracketMatch(tests[2]) == false);

    /* 中缀转后缀 + 计算测试 */
    const char *expr = "3+(5-2)*4";
    char postfix[128];
    InfixToPostfix(expr, postfix, sizeof(postfix));
    int result = EvalPostfix(postfix);
    printf("中缀：%s → 后缀：%s= 计算结果：%d（期望15）\n",
           expr, postfix, result);
    assert(result == 15);

    Stack_Destroy(&S);
    printf("✅ 栈测试全部通过！\n\n");
}
