#ifndef LISTA_HPP
#define LISTA_HPP

#include <cstddef> // pra size_t

struct Celula_Lista {
    int letra;
    Celula_Lista* prox;
    Celula_Lista* ant;
};

struct Lista {
    Celula_Lista* topo;
    Celula_Lista* ultima;
    size_t tamanho;
};

void IniciarLista(Lista* list);
void adicionarCelulaFinalLista(Lista* list, int letra);
void adicionarCelulaInicioLista(Lista* list, int letra);
bool adicionarCelulaLista(Lista* list, int letra, size_t indice);
int  removerCelulaLista(Lista* list, size_t indice, bool* ok);
void ImprimirLista(const Lista& list);
void LimparLista(Lista* list);

#endif
