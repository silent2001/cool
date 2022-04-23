#include<stdio.h>
#include<stdlib.h>
#define true 1
#define false 0
#define MAX_VEXTEX_NUM 20
#define M 20
#define STACK_INIT_SIZE 100
#define STACKINCREMENT 10
/*----------------------- ͼ���ڽӱ�洢�ṹ ------------------------*/
typedef struct ArcNode /* �����ṹ���� */
{
    int adjvex; /* �û�ָ��Ķ����λ�� */
    struct ArcNode* nextarc; /* ָ����һ������ָ�� */
}ArcNode;
typedef struct VNode /* �ڽӱ�ͷ������� */
{
    int data; /* ������Ϣ */
    ArcNode* firstarc; /* ָ���һ�������ڸõ�Ļ���ָ�� */
}VNode, AdjList[MAX_VEXTEX_NUM]; /*AdjList Ϊ�ڽӱ����� */
typedef struct
{
    AdjList vertices;
    int vexnum, arcnum;
}ALGraph;
/*----------------------------------------------------------------*/
void CreatGraph(ALGraph* G) /* ͨ���û���������һ��ͼ���ڽӱ� */
{
    int m, n, i;
    ArcNode* p;
    printf("=======================================================");
    printf("\n ���붥���� :");
    scanf_s("%d", &G->vexnum);
    printf("\n ������� :");
    scanf_s("%d", &G->arcnum);
    printf("=======================================================");
    for (i = 1; i <= G->vexnum; i++) /* ��ʼ�������� */
    {
        G->vertices[i].data = i; /* ��д�����λ����� */
        G->vertices[i].firstarc = NULL;
    }
    for (i = 1; i <= G->arcnum; i++) /* ��¼ͼ��������ȷ���Ļ� */
    {
        printf("\n ����ȷ�������������� u �� v:");
        scanf_s("%d %d", &n, &m);
        while (n<0 || n>G->vexnum || m<0 || m>G->vexnum)
        {
            printf(" ����Ķ�����Ų���ȷ���������� :");
            scanf_s("%d%d", &n, &m);
        }
        p = (ArcNode*)malloc(sizeof(ArcNode)); /* �����µĻ�������洢�û�����Ļ���Ϣ */
        if (p == NULL)
        {
            printf("ERROR!");
            exit(1);
        }
        p->adjvex = m; /* �û�ָ��λ�ñ��Ϊ m �Ľ�� */
        p->nextarc = G->vertices[n].firstarc;
        /* ��һ����ָ����������� n �ĵ�һ���� */
        G->vertices[n].firstarc = p;
    }
    printf("=======================================================");
    printf("\n �������ڽӱ�Ϊ :\n");
    /* ��ӡ���ɵ��ڽӱ���һ���ĸ�ʽ�� */
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
typedef struct /* ջ�Ĵ洢�ṹ */
{
    int* base; /* ջ��ָ�� */
    int* top; /* ջ��ָ�� */
    int stacksize;
}SqStack;
/*----------------------------------------------------------------*/
void InitStack(SqStack* S) /* ��ʼ��ջ */
{
    S->base = (int*)malloc(STACK_INIT_SIZE * sizeof(int));
    if (!S->base) /* �洢����ʧ�� */
    {
        printf("ERROR!");
        exit(1);
    }
    S->top = S->base;
    S->stacksize = STACK_INIT_SIZE;
}
/*----------------------------------------------------------------*/
void Push(SqStack* S, int e) /* ѹ���µ�Ԫ��Ϊջ�� */
{
    if (S->top - S->base >= S->stacksize)
    {
        S->base = (int*)realloc(S->base, (S->stacksize + STACKINCREMENT) * sizeof(int)); /* ׷���¿ռ� */
        if (!S->base) /* �洢����ʧ�� */
        {
            printf("ERROR!");
            exit(1);
        }
        S->top = S->base + S->stacksize;
        S->stacksize += STACKINCREMENT;
    }
    *S->top++ = e; /*e ��Ϊ�µ�ջ��Ԫ�� */
}
/*----------------------------------------------------------------*/
int Pop(SqStack* S, int* e) /* ����ջ������ e ���� */
{
    if (S->top == S->base) /* ջΪ�� */
    {
        return false;
    }
    *e = *--S->top;
    return 0;
}
/*----------------------------------------------------------------*/
int StackEmpty(SqStack* S) /* �ж�ջ�Ƿ�Ϊ�գ�Ϊ�շ��� 1 ����Ϊ�շ��� 0*/
{
    if (S->top == S->base)
        return true;
    else
        return false;
}
/*----------------------------------------------------------------*/
void FindInDegree(ALGraph G, int indegree[]) /* �Ը���������� */
{
    int i;
    for (i = 1; i <= G.vexnum; i++) /* ��ȸ���ֵ 0*/
    {
        indegree[i] = 0;
    }
    for (i = 1; i <= G.vexnum; i++)
    {
        while (G.vertices[i].firstarc)
        {
            indegree[G.vertices[i].firstarc->adjvex]++;
            /* ���Ȳ�Ϊ�㣬��ö��� firstarc ��ָ��Ļ�ָ��Ķ�����ȼ�һ */
            G.vertices[i].firstarc = G.vertices[i].firstarc->nextarc;
        }
    }
}
/*----------------------------------------------------------------*/
void TopoSort(ALGraph G)
{
    int indegree[M];
    int i, k, n;
    int count = 0; /* ��ʼ����������� */
    ArcNode* p;
    SqStack S;
    FindInDegree(G, indegree);
    InitStack(&S);
    for (i = 1; i <= G.vexnum; i++)
    {
        printf("\n");
        printf("indegree[%d] = %d \n", i, indegree[i]); /* ������ */
    }
    printf("\n");
    for (i = 1; i <= G.vexnum; i++) /* ���Ϊ 0 ����ջ */
    {
        if (!indegree[i])
            Push(&S, i);
    }
    printf("=======================================================");
    printf("\n\n ������������Ϊ :");
    while (!StackEmpty(&S)) /* ջ��Ϊ�� */
    {
        Pop(&S, &n); /* ����ջ�� */
        printf("%4d", G.vertices[n].data); /* ���ջ�������� */
        count++;
        for (p = G.vertices[n].firstarc; p != NULL; p = p->nextarc)
            /*n �Ŷ����ÿ���ڽӵ���ȼ�һ */
        {
            k = p->adjvex;
            if (!(--indegree[k])) /* ����ȼ�Ϊ�㣬������ջ */
            {
                Push(&S, k);
            }
        }
    }
    if (count < G.vexnum)/* ���������С��ԭʼͼ�Ķ�����������ͼ���л�· */
    {
        printf("ERROR  ���ִ���!\n");
        printf("                  ���ݿ����������");
    }
    else
    {
        printf("  ����ɹ� !\n");
        printf("                  ���ݿⲻ����������");
    }
}
/*----------------------------------------------------------------*/
int main(void) /* ��д���������Ե��������������� */
{
    ALGraph G;
    CreatGraph(&G); /* �����ڽӱ� */
    TopoSort(G); /* ��ͼ G ������������ */
    printf("\n\n");
    system("pause");
    /* ����ϵͳ�� dos ��� pause; ��ʾ :" ����������� ..."*/
    return 0;
}