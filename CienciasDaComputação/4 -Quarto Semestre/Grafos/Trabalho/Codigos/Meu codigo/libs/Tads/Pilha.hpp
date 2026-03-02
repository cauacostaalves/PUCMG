#ifndef PILHA_HPP
#define PILHA_HPP

#include <cstddef> // size_t

struct Celula_Pilha {
    int valor;
    Celula_Pilha* prox;
};

struct Pilha {
    Celula_Pilha* topo;
    size_t tamanho;
};

void IniciarPilha(Pilha* p);
void Empilhar(Pilha* p, int valor);                 // push
int  Desempilhar(Pilha* p, bool* ok);               // pop; ok=true se removeu
int  TopoPilha(const Pilha* p, bool* ok);           // peek; ok=true se existe
bool PilhaVazia(const Pilha* p);
size_t PilhaTamanho(const Pilha* p);
void ImprimirPilha(const Pilha& p);                 // imprime do topo ao fundo
void LimparPilha(Pilha* p);                         // free total

#endif // PILHA_HPP
