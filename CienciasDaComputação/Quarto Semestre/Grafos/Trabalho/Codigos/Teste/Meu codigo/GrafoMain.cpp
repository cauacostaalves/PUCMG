#include <iostream>
#include "libs/headers.hpp"

using namespace std;

void menu_Matriz(int vert){
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
        Vertice_NDP **grafo = new Vertice_NDP *[vert];
        IniciarGrafo_NDP(grafo, vert);
        ImprimirGrafo_NDP(grafo, vert);
        CriarArestas_NDP(grafo, vert);
        ImprimirGrafo_NDP(grafo, vert);
        for (int i = 0; i < vert; ++i) delete[] grafo[i];
        delete[] grafo;

    } else if (tipo == 3) {
        bool **grafo = new bool*[vert];
        IniciarGrafo_DNP(grafo, vert);
        ImprimirGrafo_DNP(grafo, vert);
        CriarArestas_DNP(grafo, vert);
        ImprimirGrafo_DNP(grafo, vert);
        for (int i = 0; i < vert; ++i) delete[] grafo[i];
        delete[] grafo;

    } else if (tipo == 4) {
        Vertice_DP **grafo = new Vertice_DP *[vert];
        IniciarGrafo_DP(grafo, vert);
        ImprimirGrafo_DP(grafo, vert);
        CriarArestas_DP(grafo, vert);
        ImprimirGrafo_DP(grafo, vert);
        for (int i = 0; i < vert; ++i) delete[] grafo[i];
        delete[] grafo;
        
    } else {
        cout << "Opção inválida." << endl;
    }
}

void menu_Lista(int vert){
    int tipo;
    cout << "Selecione o tipo de grafo: " << endl;
    cout << "1 - Grafo Nao Direcionado e Nao Ponderado" << endl;
    cout << "2 - Grafo Nao Direcionado e Ponderado" << endl;
    cout << "3 - Grafo Direcionado e Nao Ponderado" << endl;
    cout << "4 - Grafo Direcionado e Ponderado" << endl;
    cin >> tipo;

    if(tipo == 1){
        // bool **grafo = new bool*[vert];
        // IniciarGrafo_NDNP(grafo, vert);
        // ImprimirGrafo_NDNP(grafo, vert);
        // CriarArestas_NDNP(grafo, vert);
        // ImprimirGrafo_NDNP(grafo, vert);
        // for (int i = 0; i < vert; ++i) delete[] grafo[i];
        // delete[] grafo;

    }else if(tipo == 2){
        // Vertice_NDP **grafo = new Vertice_NDP *[vert];
        // IniciarGrafo_NDP(grafo, vert);
        // ImprimirGrafo_NDP(grafo, vert);
        // CriarArestas_NDP(grafo, vert);
        // ImprimirGrafo_NDP(grafo, vert);
        // for (int i = 0; i < vert; ++i) delete[] grafo[i];
        // delete[] grafo;

    } else if (tipo == 3) {
        // bool **grafo = new bool*[vert];
        // IniciarGrafo_DNP(grafo, vert);
        // ImprimirGrafo_DNP(grafo, vert);
        // CriarArestas_DNP(grafo, vert);
        // ImprimirGrafo_DNP(grafo, vert);
        // for (int i = 0; i < vert; ++i) delete[] grafo[i];
        // delete[] grafo;

    } else if (tipo == 4) {
        // Vertice_DP **grafo = new Vertice_DP *[vert];
        // IniciarGrafo_DP(grafo, vert);
        // ImprimirGrafo_DP(grafo, vert);
        // CriarArestas_DP(grafo, vert);
        // ImprimirGrafo_DP(grafo, vert);
        // for (int i = 0; i < vert; ++i) delete[] grafo[i];
        // delete[] grafo;
        
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

    cout << "Escolha o tipo do grafo:" << endl;
    cout << "1 - Grafo por matriz" << endl;
    cout << "2 - Grafo por lista" << endl;
    int op;
    cin >> op;
    if(op == 1){
        menu_Matriz(vert);
    }else if(op == 2){
        menu_Lista(vert);
    }else{
        cout << "Opção invalida" << endl;
    }
    
    return 0;
}
