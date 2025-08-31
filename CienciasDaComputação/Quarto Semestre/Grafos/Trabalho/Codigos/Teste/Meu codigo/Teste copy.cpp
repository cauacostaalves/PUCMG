#include <iostream>
#include "libs/headers.hpp"

using namespace std;

void menu_Matriz(int vert){
    while(true){
        int tipo;
        cout << "Selecione o tipo de grafo: " << endl;
        cout << "1 - Grafo Nao Direcionado e Nao Ponderado" << endl;
        cout << "2 - Grafo Nao Direcionado e Ponderado" << endl;
        cout << "3 - Grafo Direcionado e Nao Ponderado" << endl;
        cout << "4 - Grafo Direcionado e Ponderado" << endl;
        cout << "0 - Voltar para o menu de representacao" << endl;
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

        }else if( tipo == 0){
            return;
        } else {
            cout << "Opção invalida." << endl;
        }
    }
}

void menu_Lista(int vert){
     while(true){
        int tipo;
        cout << "Selecione o tipo de grafo: " << endl;
        cout << "1 - Grafo Nao Direcionado e Nao Ponderado" << endl;
        cout << "2 - Grafo Nao Direcionado e Ponderado" << endl;
        cout << "3 - Grafo Direcionado e Nao Ponderado" << endl;
        cout << "4 - Grafo Direcionado e Ponderado" << endl;
        cout << "0 - Voltar para o menu de repsentacao" << endl;
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

        }else if( tipo == 0){
            return;
        } else {
            cout << "Opção invalida." << endl;
        }
    }
}

void menu_RepresentacaoGrafo(){
    while(true){
        int op;
        cout << "Escolha a representacao do grafo:" << endl;
        cout << "1 - Grafo de matriz" << endl;
        cout << "2 - Grafo de lista" << endl;
        cout << "0 - Finaliza o programa" << endl;
        cin >> op;

        if(op == 0){
            cout << "Finalizando..." << endl;
            break;
        }

        int vert;
        cout << "Escreva o numero de vertices do grafo: " << endl;
        cin >> vert;
        while(vert <= 0){
            cout << "Numero invalido (Requisitos: Inteiro > 0)" << endl;
            cin >> vert;
        }

        if(op == 1){
            cout << "-------Grafo Matriz-------" << endl;
            menu_Matriz(vert);
        }else if(op == 2){
            cout << "-------Grafo Lista--------" << endl;
            menu_Lista(vert);
        }else{
            cout << "Opção invalida" << endl;
        }
    }
}

int main() {
   
    menu_RepresentacaoGrafo();
    cout << "Fim";
    
    return 0;
}
