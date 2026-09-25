/**
 * 图：邻接矩阵 + 邻接表双实现
 * 包含：BFS、DFS、Dijkstra最短路径、Floyd、Prim最小生成树、拓扑排序
 */
#ifndef GRAPH_H
#define GRAPH_H

#include "common.h"

#define MAX_VERTEX_NUM 30
#define INFINITY 0x3f3f3f3f

typedef char VertexType;  /* 顶点类型：A B C D ... */
typedef int  EdgeType;    /* 权值类型 */
typedef bool Visited[MAX_VERTEX_NUM];

/* ========== 邻接矩阵 ========== */
typedef struct {
    VertexType vexs[MAX_VERTEX_NUM];       /* 顶点表 */
    EdgeType   arcs[MAX_VERTEX_NUM][MAX_VERTEX_NUM]; /* 邻接矩阵 */
    int vexnum, arcnum;                    /* 顶点数、边数 */
    bool directed;                         /* 是否有向 */
} MGraph;

Status MGraph_Create(MGraph *G, bool directed, VertexType vexs[], int n,
                     VertexType edges[][3], int e);  /* edges: u-v-w */
int    MGraph_LocateVex(const MGraph *G, VertexType v);
void   MGraph_DFS(const MGraph *G, int v, Visited visited, VisitFunc visit);
void   MGraph_BFS(const MGraph *G, int v, VisitFunc visit);
/* Dijkstra：v0到各点最短路径，dist存距离，path存路径前驱 */
void   MGraph_Dijkstra(const MGraph *G, int v0, int dist[], int path[]);
/* Floyd：全源最短路径 */
void   MGraph_Floyd(const MGraph *G, int dist[][MAX_VERTEX_NUM], int path[][MAX_VERTEX_NUM]);
/* Prim最小生成树（从v0出发） */
void   MGraph_Prim(const MGraph *G, int v0, int sumweight[]);
/* 拓扑排序（AOV有向图）：返回拓扑序列长度 */
int    MGraph_TopologicalSort(MGraph *G, int topo[]);

/* ========== 邻接表 ========== */
typedef struct ArcNode {
    int adjvex;                 /* 邻接点下标 */
    EdgeType weight;
    struct ArcNode *nextarc;
} ArcNode;

typedef struct VNode {
    VertexType data;
    ArcNode *firstarc;
} VNode, AdjList[MAX_VERTEX_NUM];

typedef struct {
    AdjList vertices;
    int vexnum, arcnum;
    bool directed;
} ALGraph;

Status ALGraph_Create(ALGraph *G, bool directed, VertexType vexs[], int n,
                      VertexType edges[][3], int e);
void   ALGraph_Destroy(ALGraph *G);
void   ALGraph_DFS(const ALGraph *G, int v, Visited visited, VisitFunc visit);
void   ALGraph_BFS(const ALGraph *G, int v, VisitFunc visit);

#endif /* GRAPH_H */
