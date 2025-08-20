#include <iostream>
#include "libs/GrafoMatriz.hpp"

using namespace std;

int main() {
    int vert;
    cout << "Escreva o numero de vertices do grafo: " << endl;
    cin >> vert;
    while(vert <= 0){
        cout << "Numero invalido (Requisitos: Inteiro > 0)" << endl;
        cin >> vert;
    }
    
    bool **grafo = new bool *[vert];

    IniciarGrafo(grafo, vert);
    ImprimirGrafo(grafo, vert);

    CriarArestas(grafo, vert);
    ImprimirGrafo(grafo, vert);

    DFS(grafo, vert);
    
    return 0;
}
