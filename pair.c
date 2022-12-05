#include <stdio.h>
#include <stdlib.h>

typedef struct Pair{
    int i;
    int j;
} Pair;

Pair make_pair(int i, int j){
    return (Pair){i, j};
}

void print_pair(Pair *pair){
    printf("i: %d, j: %d \n", pair->i, pair->j);
}

int main(){
    Pair newpair = make_pair(3,4);
    print_pair(&newpair);
}

