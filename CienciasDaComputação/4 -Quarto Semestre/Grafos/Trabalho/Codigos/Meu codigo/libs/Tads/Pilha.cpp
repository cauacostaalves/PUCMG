#include "Pilha.hpp"
#include <iostream>

using namespace std;

void IniciarPilha(Pilha* p){
    p->topo = nullptr;
    p->tamanho = 0;
}

void Empilhar(Pilha* p, int valor){
    auto* nova = new Celula_Pilha{valor, p->topo};
    p->topo = nova;
    p->tamanho++;
}

int Desempilhar(Pilha* p, bool* ok){
    if (!p->topo){
        if (ok) *ok = false;
        cout << "Pilha vazia!\n";
        return 0;
    }
    auto* rem = p->topo;
    int v = rem->valor;
    p->topo = rem->prox;
    delete rem;
    p->tamanho--;
    if (ok) *ok = true;
    return v;
}

int TopoPilha(const Pilha* p, bool* ok){
    if (!p->topo){
        if (ok) *ok = false;
        cout << "Pilha vazia!\n";
        return 0;
    }
    if (ok) *ok = true;
    return p->topo->valor;
}

bool PilhaVazia(const Pilha* p){
    return p->topo == nullptr;
}

size_t PilhaTamanho(const Pilha* p){
    return p->tamanho;
}

void ImprimirPilha(const Pilha& p){
    if (!p.topo){
        cout << "Pilha vazia!\n";
        return;
    }
    cout << "Topo -> [";
    for (auto* it = p.topo; it; it = it->prox){
        cout << it->valor;
        if (it->prox) cout << ", ";
    }
    cout << "] <- Fundo  (tam=" << p.tamanho << ")\n";
}

void LimparPilha(Pilha* p){
    while (p->topo){
        auto* nxt = p->topo->prox;
        delete p->topo;
        p->topo = nxt;
    }
    p->tamanho = 0;
}
