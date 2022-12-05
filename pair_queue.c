// BFS algorithm in C

#include <stdio.h>
#include <stdlib.h>
#define SIZE 40
#define QUEUE_SIZE 100

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
    printf("(i: %d, j: %d) \n", pair.i, pair.j);
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

int main(){
    queueInit();
    put(make_pair(7,8));
    put(make_pair(2,3));
    put(make_pair(3,0));
    print_pair(get());
    print_pair(get());
    print_pair(get());
    if(queueEmpty){
        printf("Empty\n");
    }
}
