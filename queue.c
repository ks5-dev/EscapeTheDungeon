// BFS algorithm in C

#include <stdio.h>
#include <stdlib.h>
#define SIZE 40

typedef struct Pair{
    int i;
    int j;
} Pair;

Pair make_pair(int i, int j){
    return (Pair){i, j};
}

void print_pair(Pair *pair){
    printf("(i: %d, j: %d) \n", pair->i, pair->j);
}

struct queue {
  Pair items[SIZE];
  Pair front;
  Pair rear;
};

struct queue* createQueue();
void enqueue(struct queue* q, Pair);
Pair dequeue(struct queue* q);
void display(struct queue* q);
int isEmpty(struct queue* q);
void printQueue(struct queue* q);

// Create a queue
struct queue* createQueue() {
  struct queue* q = malloc(sizeof(struct queue));
  q->front = (Pair){-1, -1};
  q->rear = (Pair){-1, -1};
  return q;
}

// Check if the queue is empty
int isEmpty(struct queue* q) {
  if (q->rear == (Pair){-1, -1})
    return 1;
  else
    return 0;
}

// Adding elements into queue
void enqueue(struct queue* q, int value) {
  if (q->rear == SIZE - 1)
    printf("\nQueue is Full!!");
  else {
    if (q->front == -1)
      q->front = 0;
    q->rear++;
    q->items[q->rear] = value;
  }
}

// Removing elements from queue
int dequeue(struct queue* q) {
  int item;
  if (isEmpty(q)) {
    printf("Queue is empty");
    item = -1;
  } else {
    item = q->items[q->front];
    q->front++;
    if (q->front > q->rear) {
      printf("Resetting queue ");
      q->front = q->rear = -1;
    }
  }
  return item;
}

// Print the queue
void printQueue(struct queue* q) {
  int i = q->front;

  if (isEmpty(q)) {
    printf("Queue is empty");
  } else {
    printf("\nQueue contains \n");
    for (i = q->front; i < q->rear + 1; i++) {
      printf("%d ", q->items[i]);
    }
  }
}

int main() {
  

  return 0;
}