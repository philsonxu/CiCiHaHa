#include <stdio.h>
#include <assert.h>
#include "string_ds.h"

void test_string() {
    printf("===== 测试5：串 HString 与 KMP =====\n");
    HString S, T, Sub, Con;
    String_Init(&S, "ababcabcacbab");
    String_Init(&T, "abcac");
    printf("主串S = "); String_Print(&S); printf("  长度=%d\n", String_Length(&S));
    printf("模式串T = "); String_Print(&T); printf("  长度=%d\n", String_Length(&T));

    /* BF匹配 */
    int pos_bf = BF_Index(&S, &T, 1);
    printf("BF朴素匹配位置：%d\n", pos_bf);  /* 期望 6 */
    assert(pos_bf == 6);

    /* KMP匹配 */
    int pos_kmp = KMP_Index(&S, &T, 1);
    printf("KMP匹配位置：%d\n", pos_kmp);   /* 期望 6 */
    assert(pos_kmp == 6);

    /* next数组输出 */
    int next[10], nextval[10];
    KMP_GetNext(&T, next);
    KMP_GetNextVal(&T, nextval);
    printf("next数组：    ");
    for (int i = 1; i <= T.length; i++) printf("%d ", next[i]);
    printf("\nnextval数组： ");
    for (int i = 1; i <= T.length; i++) printf("%d ", nextval[i]);
    printf("\n");

    /* 子串截取 */
    String_Init(&Sub, "");
    String_SubString(&Sub, &S, 3, 5);
    printf("从S第3位取5个字符："); String_Print(&Sub); printf("\n");  /* abcab */
    assert(Sub.length == 5);

    /* 拼接 */
    HString a, b;
    String_Init(&a, "Hello,");
    String_Init(&b, "World!");
    String_Init(&Con, "");
    String_Concat(&Con, &a, &b);
    printf("拼接结果："); String_Print(&Con); printf("\n");  /* Hello,World! */

    /* 比较 */
    HString s1, s2;
    String_Init(&s1, "apple");
    String_Init(&s2, "apply");
    printf("compare(apple, apply) = %d\n", String_Compare(&s1, &s2));  /* 负值 */

    String_Destroy(&S); String_Destroy(&T); String_Destroy(&Sub);
    String_Destroy(&Con); String_Destroy(&a); String_Destroy(&b);
    String_Destroy(&s1); String_Destroy(&s2);
    printf("✅ 串与KMP测试全部通过！\n\n");
}
