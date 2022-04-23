#include<stdio.h>
#include<stdlib.h>
#define true 1
#define false 0
#define MAX_VEXTEX_NUM 20
#define M 20
#define STACK_INIT_SIZE 100
#define STACKINCREMENT 10
/*----------------------- 图的邻接表存储结构 ------------------------*/
typedef struct ArcNode /* 弧结点结构类型 */
{
    int adjvex; /* 该弧指向的顶点的位置 */
    struct ArcNode* nextarc; /* 指向下一条弧的指针 */
}ArcNode;
typedef struct VNode /* 邻接表头结点类型 */
{
    int data; /* 顶点信息 */
    ArcNode* firstarc; /* 指向第一条依附于该点的弧的指针 */
}VNode, AdjList[MAX_VEXTEX_NUM]; /*AdjList 为邻接表类型 */
typedef struct
{
    AdjList vertices;
    int vexnum, arcnum;
}ALGraph;
/*----------------------------------------------------------------*/
void CreatGraph(ALGraph* G) /* 通过用户交互产生一个图的邻接表 */
{
    int m, n, i;
    ArcNode* p;
    printf("=======================================================");
    printf("\n 输入顶点数 :");
    scanf_s("%d", &G->vexnum);
    printf("\n 输入边数 :");
    scanf_s("%d", &G->arcnum);
    printf("=======================================================");
    for (i = 1; i <= G->vexnum; i++) /* 初始化各顶点 */
    {
        G->vertices[i].data = i; /* 编写顶点的位置序号 */
        G->vertices[i].firstarc = NULL;
    }
    for (i = 1; i <= G->arcnum; i++) /* 记录图中由两点确定的弧 */
    {
        printf("\n 输入确定弧的两个顶点 u ， v:");
        scanf_s("%d %d", &n, &m);
        while (n<0 || n>G->vexnum || m<0 || m>G->vexnum)
        {
            printf(" 输入的顶点序号不正确请重新输入 :");
            scanf_s("%d%d", &n, &m);
        }
        p = (ArcNode*)malloc(sizeof(ArcNode)); /* 开辟新的弧结点来存储用户输入的弧信息 */
        if (p == NULL)
        {
            printf("ERROR!");
            exit(1);
        }
        p->adjvex = m; /* 该弧指向位置编号为 m 的结点 */
        p->nextarc = G->vertices[n].firstarc;
        /* 下一条弧指向的是依附于 n 的第一条弧 */
        G->vertices[n].firstarc = p;
    }
    printf("=======================================================");
    printf("\n 建立的邻接表为 :\n");
    /* 打印生成的邻接表（以一定的格式） */
    for (i = 1; i <= G->vexnum; i++)
    {
        printf("%d", G->vertices[i].data);
        for (p = G->vertices[i].firstarc; p; p = p->nextarc)
            printf("-->%d", p->adjvex);
        printf("\n");
    }
    printf("=======================================================");
}
/*----------------------------------------------------------------*/
typedef struct /* 栈的存储结构 */
{
    int* base; /* 栈底指针 */
    int* top; /* 栈顶指针 */
    int stacksize;
}SqStack;
/*----------------------------------------------------------------*/
void InitStack(SqStack* S) /* 初始化栈 */
{
    S->base = (int*)malloc(STACK_INIT_SIZE * sizeof(int));
    if (!S->base) /* 存储分配失败 */
    {
        printf("ERROR!");
        exit(1);
    }
    S->top = S->base;
    S->stacksize = STACK_INIT_SIZE;
}
/*----------------------------------------------------------------*/
void Push(SqStack* S, int e) /* 压入新的元素为栈顶 */
{
    if (S->top - S->base >= S->stacksize)
    {
        S->base = (int*)realloc(S->base, (S->stacksize + STACKINCREMENT) * sizeof(int)); /* 追加新空间 */
        if (!S->base) /* 存储分配失败 */
        {
            printf("ERROR!");
            exit(1);
        }
        S->top = S->base + S->stacksize;
        S->stacksize += STACKINCREMENT;
    }
    *S->top++ = e; /*e 作为新的栈顶元素 */
}
/*----------------------------------------------------------------*/
int Pop(SqStack* S, int* e) /* 弹出栈顶，用 e 返回 */
{
    if (S->top == S->base) /* 栈为空 */
    {
        return false;
    }
    *e = *--S->top;
    return 0;
}
/*----------------------------------------------------------------*/
int StackEmpty(SqStack* S) /* 判断栈是否为空，为空返回 1 ，不为空返回 0*/
{
    if (S->top == S->base)
        return true;
    else
        return false;
}
/*----------------------------------------------------------------*/
void FindInDegree(ALGraph G, int indegree[]) /* 对各顶点求入度 */
{
    int i;
    for (i = 1; i <= G.vexnum; i++) /* 入度赋初值 0*/
    {
        indegree[i] = 0;
    }
    for (i = 1; i <= G.vexnum; i++)
    {
        while (G.vertices[i].firstarc)
        {
            indegree[G.vertices[i].firstarc->adjvex]++;
            /* 出度不为零，则该顶点 firstarc 域指向的弧指向的顶点入度加一 */
            G.vertices[i].firstarc = G.vertices[i].firstarc->nextarc;
        }
    }
}
/*----------------------------------------------------------------*/
void TopoSort(ALGraph G)
{
    int indegree[M];
    int i, k, n;
    int count = 0; /* 初始化输出计数器 */
    ArcNode* p;
    SqStack S;
    FindInDegree(G, indegree);
    InitStack(&S);
    for (i = 1; i <= G.vexnum; i++)
    {
        printf("\n");
        printf("indegree[%d] = %d \n", i, indegree[i]); /* 输出入度 */
    }
    printf("\n");
    for (i = 1; i <= G.vexnum; i++) /* 入度为 0 的入栈 */
    {
        if (!indegree[i])
            Push(&S, i);
    }
    printf("=======================================================");
    printf("\n\n 拓扑排序序列为 :");
    while (!StackEmpty(&S)) /* 栈不为空 */
    {
        Pop(&S, &n); /* 弹出栈顶 */
        printf("%4d", G.vertices[n].data); /* 输出栈顶并计数 */
        count++;
        for (p = G.vertices[n].firstarc; p != NULL; p = p->nextarc)
            /*n 号顶点的每个邻接点入度减一 */
        {
            k = p->adjvex;
            if (!(--indegree[k])) /* 若入度减为零，则再入栈 */
            {
                Push(&S, k);
            }
        }
    }
    if (count < G.vexnum)/* 输出顶点数小于原始图的顶点数，有向图中有回路 */
    {
        printf("ERROR  出现错误!\n");
        printf("                  数据库存在死锁！");
    }
    else
    {
        printf("  排序成功 !\n");
        printf("                  数据库不存在死锁！");
    }
}
/*----------------------------------------------------------------*/
int main(void) /* 编写主调函数以调用上述被调函数 */
{
    ALGraph G;
    CreatGraph(&G); /* 建立邻接表 */
    TopoSort(G); /* 对图 G 进行拓扑排序 */
    printf("\n\n");
    system("pause");
    /* 调用系统的 dos 命令： pause; 显示 :" 按任意键继续 ..."*/
    return 0;
}