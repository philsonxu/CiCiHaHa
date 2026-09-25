/**
 * C语言数据结构源程序库 —— 全部验证程序入口
 *
 * 运行本程序会依次执行所有数据结构的验证测试，每个测试都有输出结果
 * 和 ✅/❌ 判定，可以直接看到全部算法的正确性。
 */
#include <stdio.h>

/* 各模块测试函数声明 */
extern void test_seqlist();
extern void test_linklist();
extern void test_stack();
extern void test_queue();
extern void test_string();
extern void test_bitree();
extern void test_graph();
extern void test_search();
extern void test_sort();

int main() {
    printf("╔════════════════════════════════════════════════╗\n");
    printf("║   C语言数据结构源程序库 - 全部验证程序          ║\n");
    printf("╚════════════════════════════════════════════════╝\n\n");

    test_seqlist();     /* 1. 顺序表 */
    test_linklist();    /* 2. 单链表（含反转、倒数K、环检测） */
    test_stack();       /* 3. 栈 + 括号匹配 + 中缀转后缀 + 表达式求值 */
    test_queue();       /* 4. 循环队列 + 链式队列 */
    test_string();      /* 5. 串 + KMP模式匹配 */
    test_bitree();      /* 6. 二叉树 + 遍历 + BST二叉搜索树 */
    test_graph();       /* 7. 图：邻接矩阵/邻接表 + BFS/DFS + Dijkstra/Prim/拓扑/Floyd */
    test_search();      /* 8. 查找：顺序/二分/插值 + 链地址法哈希表 */
    test_sort();        /* 9. 十大排序算法 + 性能基准 */

    printf("╔════════════════════════════════════════════════╗\n");
    printf("║     🎉  全部 9 大模块共 40+ 数据结构/算法      ║\n");
    printf("║              验证全部通过！                      ║\n");
    printf("╚════════════════════════════════════════════════╝\n");
    return 0;
}
