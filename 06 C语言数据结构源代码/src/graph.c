#include "graph.h"
#include "queue.h"

int MGraph_LocateVex(const MGraph *G, VertexType v) {
    for (int i = 0; i < G->vexnum; i++)
        if (G->vexs[i] == v) return i;
    return -1;
}

Status MGraph_Create(MGraph *G, bool directed, VertexType vexs[], int n,
                     VertexType edges[][3], int e) {
    G->directed = directed;
    G->vexnum = n; G->arcnum = e;
    for (int i = 0; i < n; i++) G->vexs[i] = vexs[i];
    /* 初始化邻接矩阵 */
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            G->arcs[i][j] = (i == j) ? 0 : INFINITY;
    for (int k = 0; k < e; k++) {
        int i = MGraph_LocateVex(G, edges[k][0]);
        int j = MGraph_LocateVex(G, edges[k][1]);
        int w = edges[k][2];
        G->arcs[i][j] = w;
        if (!directed) G->arcs[j][i] = w;
    }
    return OK;
}

void MGraph_DFS(const MGraph *G, int v, Visited visited, VisitFunc visit) {
    visit(G->vexs[v]); visited[v] = true;
    for (int w = 0; w < G->vexnum; w++)
        if (G->arcs[v][w] != INFINITY && G->arcs[v][w] != 0 && !visited[w])
            MGraph_DFS(G, w, visited, visit);
}

void MGraph_BFS(const MGraph *G, int v, VisitFunc visit) {
    Visited visited = {false};
    CircQueue Q; CircQueue_Init(&Q);
    visit(G->vexs[v]); visited[v] = true;
    CircQueue_Enqueue(&Q, v);
    while (!CircQueue_Empty(&Q)) {
        int u; CircQueue_Dequeue(&Q, &u);
        for (int w = 0; w < G->vexnum; w++) {
            if (G->arcs[u][w] != INFINITY && G->arcs[u][w] != 0 && !visited[w]) {
                visit(G->vexs[w]); visited[w] = true;
                CircQueue_Enqueue(&Q, w);
            }
        }
    }
}

/* 打印路径工具 */
static void print_path(int path[], int v0, int v, const MGraph *G) {
    if (v == v0) { printf("%c", G->vexs[v]); return; }
    print_path(path, v0, path[v], G);
    printf("->%c", G->vexs[v]);
}

void MGraph_Dijkstra(const MGraph *G, int v0, int dist[], int path[]) {
    bool final[MAX_VERTEX_NUM] = {false};
    for (int i = 0; i < G->vexnum; i++) {
        dist[i] = G->arcs[v0][i];
        path[i] = (dist[i] < INFINITY && dist[i] != 0) ? v0 : -1;
    }
    dist[v0] = 0; final[v0] = true;
    for (int i = 1; i < G->vexnum; i++) {
        int min = INFINITY, u = v0;
        for (int w = 0; w < G->vexnum; w++)
            if (!final[w] && dist[w] < min) { min = dist[w]; u = w; }
        final[u] = true;
        for (int w = 0; w < G->vexnum; w++) {
            if (!final[w] && G->arcs[u][w] < INFINITY && dist[u] + G->arcs[u][w] < dist[w]) {
                dist[w] = dist[u] + G->arcs[u][w];
                path[w] = u;
            }
        }
    }
    (void)print_path;
}

void MGraph_Floyd(const MGraph *G, int dist[][MAX_VERTEX_NUM], int path[][MAX_VERTEX_NUM]) {
    for (int i = 0; i < G->vexnum; i++)
        for (int j = 0; j < G->vexnum; j++) {
            dist[i][j] = G->arcs[i][j];
            path[i][j] = (dist[i][j] < INFINITY && i != j) ? i : -1;
        }
    for (int k = 0; k < G->vexnum; k++)
        for (int i = 0; i < G->vexnum; i++)
            for (int j = 0; j < G->vexnum; j++)
                if (dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    path[i][j] = path[k][j];
                }
}

void MGraph_Prim(const MGraph *G, int v0, int sumweight[]) {
    int lowcost[MAX_VERTEX_NUM], adjvex[MAX_VERTEX_NUM];
    *sumweight = 0;
    for (int i = 0; i < G->vexnum; i++) {
        lowcost[i] = G->arcs[v0][i];
        adjvex[i] = v0;
    }
    lowcost[v0] = 0;
    printf("Prim最小生成树边：");
    for (int i = 1; i < G->vexnum; i++) {
        int min = INFINITY, k = 0;
        for (int w = 0; w < G->vexnum; w++)
            if (lowcost[w] != 0 && lowcost[w] < min) { min = lowcost[w]; k = w; }
        printf("%c-%c(%d) ", G->vexs[adjvex[k]], G->vexs[k], min);
        *sumweight += min;
        lowcost[k] = 0;
        for (int w = 0; w < G->vexnum; w++)
            if (G->arcs[k][w] < lowcost[w]) {
                lowcost[w] = G->arcs[k][w];
                adjvex[w] = k;
            }
    }
    printf("\n");
}

int MGraph_TopologicalSort(MGraph *G, int topo[]) {
    /* 计算入度 */
    int indegree[MAX_VERTEX_NUM] = {0};
    for (int i = 0; i < G->vexnum; i++)
        for (int j = 0; j < G->vexnum; j++)
            if (G->arcs[i][j] < INFINITY && i != j) indegree[j]++;
    CircQueue Q; CircQueue_Init(&Q);
    for (int i = 0; i < G->vexnum; i++)
        if (indegree[i] == 0) CircQueue_Enqueue(&Q, i);
    int cnt = 0;
    while (!CircQueue_Empty(&Q)) {
        int u; CircQueue_Dequeue(&Q, &u);
        topo[cnt++] = u;
        for (int w = 0; w < G->vexnum; w++)
            if (G->arcs[u][w] < INFINITY && u != w && --indegree[w] == 0)
                CircQueue_Enqueue(&Q, w);
    }
    return cnt == G->vexnum ? cnt : -1; /* -1表示有环 */
}

/* ========== 邻接表 ========== */
static int locate(const ALGraph *G, VertexType v) {
    for (int i = 0; i < G->vexnum; i++)
        if (G->vertices[i].data == v) return i;
    return -1;
}

Status ALGraph_Create(ALGraph *G, bool directed, VertexType vexs[], int n,
                      VertexType edges[][3], int e) {
    G->directed = directed; G->vexnum = n; G->arcnum = e;
    for (int i = 0; i < n; i++) {
        G->vertices[i].data = vexs[i];
        G->vertices[i].firstarc = NULL;
    }
    for (int k = 0; k < e; k++) {
        int i = locate(G, edges[k][0]);
        int j = locate(G, edges[k][1]);
        int w = edges[k][2];
        ArcNode *p = (ArcNode*)malloc(sizeof(ArcNode));
        p->adjvex = j; p->weight = w;
        p->nextarc = G->vertices[i].firstarc;
        G->vertices[i].firstarc = p;
        if (!directed) {
            ArcNode *q = (ArcNode*)malloc(sizeof(ArcNode));
            q->adjvex = i; q->weight = w;
            q->nextarc = G->vertices[j].firstarc;
            G->vertices[j].firstarc = q;
        }
    }
    return OK;
}

void ALGraph_Destroy(ALGraph *G) {
    for (int i = 0; i < G->vexnum; i++) {
        ArcNode *p = G->vertices[i].firstarc, *q;
        while (p) { q = p->nextarc; free(p); p = q; }
        G->vertices[i].firstarc = NULL;
    }
    G->vexnum = G->arcnum = 0;
}

void ALGraph_DFS(const ALGraph *G, int v, Visited visited, VisitFunc visit) {
    visit(G->vertices[v].data); visited[v] = true;
    for (ArcNode *p = G->vertices[v].firstarc; p; p = p->nextarc)
        if (!visited[p->adjvex]) ALGraph_DFS(G, p->adjvex, visited, visit);
}

void ALGraph_BFS(const ALGraph *G, int v, VisitFunc visit) {
    Visited visited = {false};
    CircQueue Q; CircQueue_Init(&Q);
    visit(G->vertices[v].data); visited[v] = true;
    CircQueue_Enqueue(&Q, v);
    while (!CircQueue_Empty(&Q)) {
        int u; CircQueue_Dequeue(&Q, &u);
        for (ArcNode *p = G->vertices[u].firstarc; p; p = p->nextarc) {
            if (!visited[p->adjvex]) {
                visit(G->vertices[p->adjvex].data);
                visited[p->adjvex] = true;
                CircQueue_Enqueue(&Q, p->adjvex);
            }
        }
    }
}
