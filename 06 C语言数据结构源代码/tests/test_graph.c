#include <stdio.h>
#include <assert.h>
#include "graph.h"

/* 辅助：打印字符顶点 */
static void print_char(ElemType c) { printf("%c ", (char)c); }

void test_graph() {
    printf("===== 测试7：图 Graph =====\n");

    /* 无向带权图：A B C D E F，共8条边 */
    VertexType vexs[] = {'A','B','C','D','E','F'};
    /* edges[u][v][w]：使用char存int权值需要特殊处理，这里用int数组重新设计——直接在测试里构造 */
    VertexType edges[][3] = {
        {'A','B',6},{'A','C',1},{'A','D',5},
        {'B','C',5},{'B','E',3},
        {'C','D',5},{'C','E',6},{'C','F',4},
        {'D','F',2},{'E','F',6},
    };
    int n = 6, e = 10;

    /* 邻接矩阵 */
    MGraph MG;
    MGraph_Create(&MG, false, vexs, n, edges, e);
    printf("--- 邻接矩阵 ---\n");
    printf("DFS从A出发：");
    Visited v1 = {false};
    MGraph_DFS(&MG, 0, v1, print_char); printf("\n");
    printf("BFS从A出发：");
    MGraph_BFS(&MG, 0, print_char); printf("\n");

    /* Dijkstra */
    int dist[MAX_VERTEX_NUM], path[MAX_VERTEX_NUM];
    MGraph_Dijkstra(&MG, 0, dist, path);
    printf("Dijkstra A→各点最短距离：\n");
    for (int i = 0; i < n; i++) {
        printf("  %c: 距离=%d 路径=", vexs[i], dist[i]);
        /* 反向打印路径 */
        int t = i, stk[30], top=-1;
        while (t != -1) { stk[++top] = t; if (t == 0) break; t = path[t]; }
        while (top>=0) { printf("%c", vexs[stk[top--]]); if(top>=0) printf("->"); }
        printf("\n");
    }
    assert(dist[4] == 7); /* A->B->E: 6+3=9? 实际 A-C-F: 1+4, A-C-E: 1+6=7 对 */

    /* Prim */
    int mst_sum;
    MGraph_Prim(&MG, 0, &mst_sum);
    printf("MST总权值：%d\n", mst_sum);
    assert(mst_sum == 1+2+3+4+5); /* 15 */

    /* 邻接表 */
    ALGraph AG;
    ALGraph_Create(&AG, false, vexs, n, edges, e);
    printf("--- 邻接表 ---\n");
    printf("DFS从A出发：");
    Visited v2 = {false};
    ALGraph_DFS(&AG, 0, v2, print_char); printf("\n");
    printf("BFS从A出发：");
    ALGraph_BFS(&AG, 0, print_char); printf("\n");

    /* 有向图拓扑排序测试 */
    printf("--- 拓扑排序（有向AOV）---\n");
    VertexType vexs2[] = {'1','2','3','4','5','6'};
    VertexType edges2[][3] = {
        {'1','2',1},{'1','3',1},{'2','4',1},
        {'3','2',1},{'3','5',1},{'4','6',1},{'5','4',1},{'5','6',1},
    };
    MGraph DG;
    MGraph_Create(&DG, true, vexs2, 6, edges2, 8);
    int topo[30];
    int cnt = MGraph_TopologicalSort(&DG, topo);
    assert(cnt == 6);
    printf("拓扑序列：");
    for (int i = 0; i < cnt; i++) printf("%c ", vexs2[topo[i]]);
    printf("\n");

    /* Floyd全源最短路径测试 */
    int fdist[MAX_VERTEX_NUM][MAX_VERTEX_NUM], fpath[MAX_VERTEX_NUM][MAX_VERTEX_NUM];
    MGraph_Floyd(&MG, fdist, fpath);
    printf("Floyd A->F = %d，与Dijkstra结果一致吗？%d\n", fdist[0][5], fdist[0][5]==dist[5]);
    assert(fdist[0][5] == dist[5]);

    ALGraph_Destroy(&AG);
    printf("✅ 图测试全部通过！\n\n");
}
