#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>

typedef struct adjNode{
    int key;
    struct adjNode* next;
}adjNode;

typedef struct adjList{
   adjNode* head;
}adjList;
typedef struct graph{
    int v;
    adjList* array;
}graph;

typedef struct stack{
    int capacity;
    int top;
    int* element;
}stack;

stack* createStack(int v){
    stack* s = (stack* )malloc(sizeof(stack));
    s->capacity = v;
    s->top = -1;
    s->element = (int *)malloc(sizeof(int)*v);
    int i;
    for(i = 0;i<v;i++){
        s->element[i] = -1;
    }
    return s;
}

void addStack(stack* s,int a){
    int i = s->top;
    i++;
    s->element[i] = a;
    s->top = i;
}

adjNode* newAdjNode(int k){
    adjNode* newNode = (adjNode *) malloc(sizeof(adjNode));
    newNode->key = k;
    newNode->next = NULL;
    return newNode;
}

graph* newGraph(int vertices){
    graph* newgraph  = (graph *)malloc(sizeof(graph));
    newgraph->v = vertices;
    newgraph->array = (adjList *)malloc(vertices*sizeof(adjList));
    int i;
    for(i = 0;i<vertices;i++){
         newgraph->array[i].head = NULL;
    }
    return newgraph;
}

void addEdge(graph* g,int src,int dest){
    adjNode* node = newAdjNode(dest);
    node->next = g->array[src].head;
    g->array[src].head = node;
}

void deleteEdge(graph* G,int src,int dest){
    adjNode* crawl = G->array[src].head;
    adjNode* l = NULL;
    while(crawl && crawl->key != dest){
        adjNode* n = newAdjNode(crawl->key);
        n->next = l;
        l = n;
        crawl = crawl->next;
    }
    if(crawl){
        crawl = crawl->next;
        while(crawl){
            adjNode* n = newAdjNode(crawl->key);
            n->next = l;
            l = n;
            crawl = crawl->next;
        }
    G->array[src].head = l;
    }
}

void printGraph(graph* g){
    int v = g->v;
    int i;
    for(i = 0;i<v;i++){
        adjNode* crawl = g->array[i].head;
        printf("\n Adjacency list of vertex %d\n head ", i);
        while(crawl){
            printf("-> %d", crawl->key);
            crawl = crawl->next;
        }
        printf("\n");
    }
}

int getEdge(float p){
    rand();
    rand();
    double a = rand();
    if(a/ RAND_MAX < p) return 1;
    else return 0;
}

int hasEdge(graph* G,int src,int dest){
    adjNode* crawl = G->array[src].head;
    while(crawl){
        if(crawl->key == dest) return 1;
        crawl = crawl->next;
    }
    //printf("%d %d \n",src,dest);
    return -1;
}

graph* createGraph(int v,float p){
    graph* G = newGraph(v);
    int i,j;
    for(i = 0;i < v;i++){
        for(j = 0;j<v;j++){
            if( i!=j && getEdge(p) == 1 && hasEdge(G,j,i) == -1){
                addEdge(G,i,j);
            }
        }
    }
    return G;
}

int isCyclicUtil(graph* G,int s,int* visited,int* recstack){
    int i;
    if(visited[s] == -1){

            visited[s] = 1;
            recstack[s] = 1;
            adjNode* crawl = G->array[s].head;
            while(crawl){
                i = crawl->key;
                if(visited[i] == -1 && isCyclicUtil(G,i,visited,recstack)==1)
                    return 1;
                else if(recstack[i] == 1)
                    return 1;
                crawl = crawl->next;
            }
    }
    recstack[s] = -1;
    return -1;
}

int isCyclic(graph* G){
    int n = G->v;
    int* visited = (int *)malloc(n*sizeof(int));
    int* recstack = (int *)malloc(n*sizeof(int));
    int i;
    for(i = 0;i<n;i++){
        visited[i] = -1;
        recstack[i] = -1;
    }
    for(i = 0;i<n;i++){
        if(isCyclicUtil(G,i,visited,recstack) == 1)
            return 1;
    }
  return -1;
}
void topologicalUtil(graph* G,int x,int* visited,stack* s){
    visited[x] = 1;
    adjNode* crawl = G->array[x].head;
    int i;
    //printf("OK");
    while(crawl){
            i = crawl->key;
            if(visited[i] == -1)
                topologicalUtil(G,i,visited,s);
            //printf("OK1");
            crawl = crawl->next;
    }
    addStack(s,x);
}

void topological(graph* G){
    int v = G->v;
    int* visited = (int *)malloc(v*sizeof(int));
    stack* s = createStack(v);
    int i;
    for(i = 0;i<v;i++){
        visited[i] = -1;
    }
    printf("Topological Sort:\n");
    for(i = 0;i<v;i++){
            if(visited[i] == -1)
                topologicalUtil(G,i,visited,s);
    }
    for(i = v-1;i>=0;i--){
        printf("%d ",s->element[i]);
    }
    printf("\n");
}

graph* transGraph(graph* G){
    int v = G->v;
    graph* nG = createGraph(v,0);
    int i;
    for(i=0 ; i<v; i++){
        adjNode* crawl = G->array[i].head;
        while(crawl){
            addEdge(nG,crawl->key,i);
            crawl = crawl->next;
        }
    }
    return nG;
}

int sizeList(graph* G,int src){
    adjNode* crawl = G->array[src].head;
    int i = 0;
    while(crawl){
        i++;
        crawl = crawl->next;
    }
    return i;
}

void eulerTourutil(graph* G,int start,stack* s){
    addStack(s,start);
    adjNode* crawl = G->array[start].head;
    int i;
    while(crawl){
        i = crawl->key;
        deleteEdge(G,start,i);
        eulerTourutil(G,i,s);
        crawl = crawl->next;
    }
}

void  eulerTour(graph* G){
    int i = 0;
    stack* s = createStack(G->v);
    int e;
    graph* GT = transGraph(G);
    printf("Euler Tour started:\n");
    for(i = 0;i<G->v;i++){
        int x = sizeList(G,i);
        int y = sizeList(GT,i);
        if(x ==0 && y == 0)
            printf("%d ",i);
    }
    printf("\n");
    for(i = 0;i<G->v;i++){
        int x = sizeList(G,i);
        int y = sizeList(GT,i);
        if(x-y >= 0 && x!= 0 && y!=0){
            e = i;
            break;
        }
    }
    eulerTourutil(G,e,s);
    for(i = 0 ;i<G->v;i++){
        printf("%d ",s->element[i]);
    }
    printf("\n");
}

int main(int argc, char* argv[]){
    int n = atoi(argv[1]);
    float p = atof(argv[2]);
    graph* G = createGraph(n,p);
    printGraph(G);
    int cyclic = isCyclic(G);
    if(cyclic == 1){
        printf("Graph is Cyclic\n");
        eulerTour(G);
    }
    else if(cyclic == -1) {
        printf("Graph is not Cyclic\n");
        topological(G);
    }
    printGraph(G);

   /* graph* G = createGraph(5,0);
    addEdge(G,0, 1);
    addEdge(G,1, 2);
    addEdge(G,2, 0);
    addEdge(G,0, 3);
    addEdge(G,3, 4);
    addEdge(G,4, 0);
    printGraph(G);
    printf("Is cyclic %d \n",isCyclic(G));
    eulerTour(G);
    printGraph(G);*/


    return 0;
}
