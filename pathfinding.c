#include <stdio.h>
#include <stdlib.h>
#define SIZE 40
#define QUEUE_SIZE 100
#define MATSIZE 3

typedef struct Pair{
    int i;
    int j;
} Pair;

Pair queue[QUEUE_SIZE+1];
int head, tail;

Pair make_pair(int i, int j){
    return (Pair){i, j};
}

void print_pair(Pair pair){
    printf("(i: %d, j: %d)\n", pair.i, pair.j);
}

int pairEqual(Pair p1, Pair p2){
    return p1.i == p2.i && p1.j == p2.j;
}

void queueInit(){
    head = 0;
    tail = 0;
}

void put(Pair v){
    queue[tail++]= v;
    if (tail > QUEUE_SIZE){
        tail = 0;
    }
}

Pair get(){
    return queue[head++];
    if (head > QUEUE_SIZE){
        head = 0;
    }
}

int queueEmpty(){
    return head == tail;
}

void printQueue(){
  //cur_size = sizeof(queue)/sizeof(a[queue]);
  for(int i = head; i < tail; i++){
    printf("Queue contains: \n");
    print_pair(queue[i]);
  }
}

int pairToIndex(Pair pair, int matsize){
    return (pair.i * matsize) + pair.j;
}

Pair indexToPair(int t, int matsize){
    int j = t % matsize;
    int i = (t - j) / matsize;
    return (Pair){i, j};
}

struct node {
    Pair vertex;
    struct node* next;
};

struct node* createNode(Pair);
struct node* newNode;
struct Graph {
  int numVertices;
  struct node** adjLists;
  int* visited;
};

// BFS algorithm
void bfs(struct Graph* graph, Pair startVertex, Pair desVertex) {
  queueInit();
  int cnt =0;

  graph->visited[pairToIndex(startVertex, MATSIZE)] = 1;
  put(startVertex);

  while (!queueEmpty()) {
    printQueue();
    Pair currentVertex = get();
    printf("Visited: ");
    printf("%d \n", pairToIndex(currentVertex, MATSIZE));

    struct node* temp = graph->adjLists[pairToIndex(currentVertex, MATSIZE)];

    while (temp) {
      Pair adjVertex = temp->vertex;

      if (graph->visited[pairToIndex(adjVertex, MATSIZE)] == 0) {
        graph->visited[pairToIndex(adjVertex, MATSIZE)] = 1;
        put(adjVertex);
        cnt++;
      }
      temp = temp->next;
    }
    // search
    if (pairEqual(currentVertex, desVertex) ){
        printf("Found. The path is as follow: \n");
        
        for(int i = 0; i < cnt; i++){
            print_pair(indexToPair(i, MATSIZE));
        }
        printf("\n");
        break;
    }
  }
}

// Creating a node
struct node* createNode(Pair v) {
  struct node* newNode = malloc(sizeof(struct node));
  newNode->vertex = v;
  newNode->next = NULL;
  return newNode;
}

// Creating a graph
struct Graph* createGraph(int vertices) {
  struct Graph* graph = malloc(sizeof(struct Graph));
  graph->numVertices = vertices;

  graph->adjLists = malloc(vertices * sizeof(struct node*));
  graph->visited = malloc(vertices * sizeof(int));

  int i;
  for (i = 0; i < vertices; i++) {
    graph->adjLists[i] = NULL;
    graph->visited[i] = 0;
  }

  return graph;
}

// Add edge
void addEdge(struct Graph* graph, int src, int des, Pair srcpair, Pair despair) {
  // Add edge from s to d
  struct node* newNode = createNode(despair);
  newNode->next = graph->adjLists[src];
  graph->adjLists[src] = newNode;
                                                                                                        
  // Add edge from d to s
  newNode = createNode(srcpair);
  newNode->next = graph->adjLists[des];
  graph->adjLists[des] = newNode;
}

int main() {
  struct Graph* graph = createGraph(9);
  /*
  addEdge(graph, 0, 1, (Pair){0,0}, (Pair){0,1});
  addEdge(graph, 0, 2, (Pair){0,0}, (Pair){1,0} );
  addEdge(graph, 1, 3, (Pair){0,1}, (Pair){1,1} );
  addEdge(graph, 2, 3, (Pair){1,0}, (Pair){1,1} );
    */
      int newmap[3][3] = {
    {1, 1, 2},
    {1, 2, 2},
    {1, 1, 1},
  };
  int cur;
  int i,j ;
  
  for(int i = 0; i< MATSIZE - 1; i++){
    for(int j = 0; j<MATSIZE - 1; j++){
      cur = pairToIndex(make_pair(i,j), MATSIZE);
      // printf("%d \n", cur);
      
      if (newmap[i][j+1] != 2){
        addEdge(graph, cur, cur+1 ,(Pair){i,j}, (Pair){i, j+1});
    }
      if (newmap[i+1][j] != 2 ){
        addEdge(graph, cur, cur+MATSIZE, (Pair){i,j}, (Pair){i+1, j});
      }
      
    }
  }
  // At the end, cur is the pair of:          i: MATSIZE - 1 , j = MATSIZE- 1 
  i = MATSIZE-1;
  j = MATSIZE -1;
  if (newmap[i][j+1] != 2){
    addEdge(graph, cur+1, cur+1+MATSIZE ,(Pair){i,j+1}, (Pair){i+1, j+1});
  }
  if (newmap[i][j+1] != 2){
    addEdge(graph, cur+MATSIZE, cur+1+MATSIZE ,(Pair){i+1,j}, (Pair){i+1, j+1});
  }

  bfs(graph, (Pair){0,0}, (Pair){2,2});
  printf("Done\n");
  return 0;
}