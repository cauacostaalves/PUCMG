#include <iostream>
#include "Lista.hpp"

using namespace std;

void IniciarLista(Lista* list){
    list->topo   = nullptr;
    list->ultima = nullptr;
    list->tamanho = 0;
}

void adicionarCelulaFinalLista(Lista* list, int letra){
    auto* nova = new Celula_Lista{letra, nullptr, list->ultima};
    if (list->ultima) {
        list->ultima->prox = nova;
    } else {
        list->topo = nova;
    }
    list->ultima = nova;
    list->tamanho++;
}

void adicionarCelulaInicioLista(Lista* list, int letra){
    auto* nova = new Celula_Lista{letra, list->topo, nullptr};
    if (list->topo) {
        list->topo->ant = nova;
    } else {
        list->ultima = nova;
    }
    list->topo = nova;
    list->tamanho++;
}

bool adicionarCelulaLista(Lista* list, int letra, size_t indice){
    if (indice > list->tamanho) return false;
    if (indice == 0)             { adicionarCelulaInicioLista(list, letra); return true; }
    if (indice == list->tamanho) { adicionarCelulaFinalLista(list, letra);  return true; }

    Celula_Lista* cur = list->topo;
    for (size_t i = 0; i < indice; ++i) cur = cur->prox;

    auto* nova = new Celula_Lista{letra, cur, cur->ant};
    cur->ant->prox = nova;
    cur->ant = nova;
    list->tamanho++;
    return true;
}

int removerCelulaLista(Lista* list, size_t indice, bool* ok){
    if (list->tamanho == 0) {
        cout << "Lista vazia!\n";
        if (ok) *ok = false;
        return 0;
    }
    if (indice >= list->tamanho) {
        cout << "Indice invalido!\n";
        if (ok) *ok = false;
        return 0;
    }

    Celula_Lista* cur = list->topo;
    for (size_t i = 0; i < indice; ++i) cur = cur->prox;

    int letra = cur->letra;

    if (cur->ant) cur->ant->prox = cur->prox; else list->topo = cur->prox;
    if (cur->prox) cur->prox->ant = cur->ant; else list->ultima = cur->ant;

    delete cur;
    list->tamanho--;
    if (ok) *ok = true;
    return letra;
}

void ImprimirLista(const Lista& list){
    if (!list.topo) {
        cout << "Lista vazia!\n";
        return;
    }
    cout << "[";
    for (auto* p = list.topo; p; p = p->prox) {
        cout << p->letra;
        if (p->prox) cout << ", ";
    }
    cout << "]  (tam=" << list.tamanho << ")\n";
}

void LimparLista(Lista* list){
    auto* p = list->topo;
    while (p) {
        auto* nxt = p->prox;
        delete p;
        p = nxt;
    }
    list->topo = list->ultima = nullptr;
    list->tamanho = 0;
}
