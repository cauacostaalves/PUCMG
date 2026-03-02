#ifndef FILA_HPP
#define FILA_HPP

#include <cstddef> // size_t

struct Celula_Fila {
    int valor;
    Celula_Fila* prox;
};

struct Fila {
    Celula_Fila* frente; // head
    Celula_Fila* tras;   // tail
    size_t tamanho;
};

void IniciarFila(Fila* f);
void Enfileirar(Fila* f, int valor);                  // push
int  Desenfileirar(Fila* f, bool* ok);                // pop; ok=true se removeu
int  FrenteFila(const Fila* f, bool* ok);             // peek; ok=true se existe
bool FilaVazia(const Fila* f);
size_t FilaTamanho(const Fila* f);
void ImprimirFila(const Fila& f);                     // frente -> ... -> tras
void LimparFila(Fila* f);                             // free total

#endif // FILA_HPP
