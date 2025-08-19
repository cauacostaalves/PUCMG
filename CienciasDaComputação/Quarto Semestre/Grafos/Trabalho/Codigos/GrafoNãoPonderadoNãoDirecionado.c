#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Celula da pilha agora armazena o numero do vertice
typedef struct Celula {
    int num_vertice;
    struct Celula *prox;
    stuct Celula *vizinho;
} Celula;

// Estrutura da Fila
typedef struct Fila {
    Celula *topo;
} Fila;

Fila* iniciarfila(){
    Fila *fila = malloc(sizeof(Fila));
    if (fila != NULL) {
        fila->topo = NULL;
    }
    return fila;
}


int main(){


    return 0;
}