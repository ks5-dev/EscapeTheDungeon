#include <stdio.h>
#include <stdlib.h>

#define VERTICAL_MATRIX_MOVE 6

int MatrixMap[7][7] = {
    {1, 0, 1, 0, 1, 0, 1},
    {1, 0, 1, 0, 2, 0, 1},
    {1, 0, 1, 2, 1, 0, 1},
    {1, 0, 2, 0, 1, 0, 1},
    {1, 0, 1, 0, 1, 0, 1},
    {1, 2, 1, 0, 1, 0, 1},
    {1, 0, 1, 0, 1, 0, 1}
};

typedef struct Pair{
    int i;
    int j;
} Pair;

Pair make_pair(int i, int j){
    return (Pair){i, j};
}

void print_pair(Pair *pair){
    printf("(i: %d, j: %d) ", pair->i, pair->j);
}


// Each node has a vertex and a pointer to one of its adjacent elements (which will then creat a link list in a graph)
struct node {
    Pair vertex;
    struct node* next;
};

struct node* createNode(Pair);
struct node* newNode;

// A graph has the number of vertices declared and a pointer to a list of struct node* (adj list). 
// The number of nodes in this linked list is unknown till run time, so we don't create it directly at compile time
struct Graph
{
    int numVertices;
    struct node** adjLists; // Note that the index represents a vertex
};

struct Graph* graph;

// Create a single empty node with a null pointer (can be replaced by a pointer to itself)
struct node* createNode(Pair v)
{
    newNode = (struct node*) malloc(sizeof(struct node));
    newNode->vertex = v;
    newNode->next = NULL;
    return newNode;
};

// Create a graph with a set number of vertices
struct Graph* createGraph(int vertices)
{
    graph = (struct Graph*) malloc(sizeof(struct Graph));
    graph->numVertices = vertices;

    graph->adjLists = malloc(vertices * sizeof(struct node*)); // The amount of memory to allocate is the total size of all vertices (in this case, node)

    int i;
    for(i = 0; i < vertices; i++){
        graph->adjLists[i] =NULL; // Upon initializing, no vertex is linked yet
    }
    return graph;
};



// Now there is a edge between vertex s and vertex d
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

// Print the graph
void printGraph(struct Graph* graph) {
  int v;
  for (v = 0; v < graph->numVertices; v++) {
    struct node* temp = graph->adjLists[v];
    printf("\n Vertex %d\n: ", v);
    while (temp) {
        print_pair(&temp->vertex);
      printf(" -> ", temp->vertex);
      temp = temp->next;
    }
    printf("\n");
  }
}

int main() {
  struct Graph* graph = createGraph(4);
  addEdge(graph, 0, 1, (Pair){0,0}, (Pair){0,1});
  addEdge(graph, 1, 2, (Pair){0,1}, (Pair){0,2} );
  addEdge(graph, 2, 3, (Pair){0,2}, (Pair){0,3} );
  addEdge(graph, 0, 4, (Pair){0,0}, (Pair){1,0} );

  printGraph(graph);

  return 0;
}

