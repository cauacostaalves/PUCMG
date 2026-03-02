#include "Fila.hpp"
#include <iostream>

using namespace std;

void IniciarFila(Fila* f){
    f->frente = nullptr;
    f->tras = nullptr;
    f->tamanho = 0;
}

void Enfileirar(Fila* f, int valor){
    auto* nova = new Celula_Fila{valor, nullptr};
    if (f->tras){
        f->tras->prox = nova;
    } else {
        f->frente = nova; // fila vazia
    }
    f->tras = nova;
    f->tamanho++;
}

int Desenfileirar(Fila* f, bool* ok){
    if (!f->frente){
        if (ok) *ok = false;
        cout << "Fila vazia!\n";
        return 0;
    }
    auto* rem = f->frente;
    int v = rem->valor;
    f->frente = rem->prox;
    if (!f->frente) f->tras = nullptr; // esvaziou
    delete rem;
    f->tamanho--;
    if (ok) *ok = true;
    return v;
}

int FrenteFila(const Fila* f, bool* ok){
    if (!f->frente){
        if (ok) *ok = false;
        cout << "Fila vazia!\n";
        return 0;
    }
    if (ok) *ok = true;
    return f->frente->valor;
}

bool FilaVazia(const Fila* f){
    return f->frente == nullptr;
}

size_t FilaTamanho(const Fila* f){
    return f->tamanho;
}

void ImprimirFila(const Fila& f){
    if (!f.frente){
        cout << "Fila vazia!\n";
        return;
    }
    cout << "Frente -> [";
    for (auto* it = f.frente; it; it = it->prox){
        cout << it->valor;
        if (it->prox) cout << ", ";
    }
    cout << "] <- Tras  (tam=" << f.tamanho << ")\n";
}

void LimparFila(Fila* f){
    while (f->frente){
        auto* nxt = f->frente->prox;
        delete f->frente;
        f->frente = nxt;
    }
    f->tras = nullptr;
    f->tamanho = 0;
}
