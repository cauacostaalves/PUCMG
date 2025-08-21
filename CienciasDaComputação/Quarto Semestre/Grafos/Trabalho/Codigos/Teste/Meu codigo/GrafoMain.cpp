#include <iostream>
#include "libs/headers.hpp"

using namespace std;

void menu(int vert){
    int tipo;
    cout << "Selecione o tipo de grafo: " << endl;
    cout << "1 - Grafo Nao Direcionado e Nao Ponderado" << endl;
    cout << "2 - Grafo Nao Direcionado e Ponderado" << endl;
    cout << "3 - Grafo Direcionado e Nao Ponderado" << endl;
    cout << "4 - Grafo Direcionado e Ponderado" << endl;
    cin >> tipo;

    if(tipo == 1){
        bool **grafo = new bool*[vert];
        IniciarGrafo_NDNP(grafo, vert);
        ImprimirGrafo_NDNP(grafo, vert);
        CriarArestas_NDNP(grafo, vert);
        ImprimirGrafo_NDNP(grafo, vert);
        
        for (int i = 0; i < vert; ++i) delete[] grafo[i];
        delete[] grafo;

    }else if(tipo == 2){
        Vertice **grafo = new Vertice *[vert];
        IniciarGrafo_NDP(grafo, vert);
        ImprimirGrafo_NDP(grafo, vert);
        CriarArestas_NDP(grafo, vert);
        ImprimirGrafo_NDP(grafo, vert);
        
        for (int i = 0; i < vert; ++i) delete[] grafo[i];
        delete[] grafo;

    } else if (tipo == 3) {
        cout << "Implementar grafo direcionado não ponderado..." << endl;
        // mesma lógica
    } else if (tipo == 4) {
        cout << "Implementar grafo direcionado ponderado..." << endl;
        // mesma lógica
    } else {
        cout << "Opção inválida." << endl;
    }
}

int main() {
    int vert;
    cout << "Escreva o numero de vertices do grafo: " << endl;
    cin >> vert;
    while(vert <= 0){
        cout << "Numero invalido (Requisitos: Inteiro > 0)" << endl;
        cin >> vert;
    }

    menu(vert);
    
    return 0;
}
