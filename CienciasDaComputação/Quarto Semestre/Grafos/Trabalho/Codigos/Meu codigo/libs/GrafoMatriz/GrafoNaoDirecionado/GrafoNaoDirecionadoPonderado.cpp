#include <iostream>
#include "GrafoNaoDirecionadoPonderado.hpp"
using namespace std;

void IniciarGrafo_NDP(Vertice_NDP **grafo, int vert){
    for(int i=0;i<vert;i++){
        grafo[i] = new Vertice_NDP[vert];
        for(int j=0;j<vert;j++){
            grafo[i][j].visitado = false;
            grafo[i][j].peso = 0;
            grafo[i][j].aresta = false;
        }
    }
}

void CriarArestas_NDP(Vertice_NDP **grafo, int vert){
    cout << "Digite os dois Vertice_NDPs que vao ser conectados e o seu peso (Digite 0 0 0 para sair)." << endl;
    char v1,v2;
    int peso;
    int maxvert = (vert*(vert-1))/2;
    while (maxvert > 0)
    {
        cin >> v1 >> v2 >> peso;

        if (v1 == '0' && v2 == '0' && peso == 0)
        {
            break;
        }

        if (v1 == v2)
        {
            cout << "Loops nao existem em um grafo simples! Por favor escreva uma aresta valida." << endl;
            continue;
        }

        if (v1 < 'A' || v1 >= 'A' + vert || v2 < 'A' || v2 >= 'A' + vert)
        {
            cout << "Vertice_NDPs invalidas! Por favor escreva arestas entre A e " << char('A' + vert - 1) << endl;
            continue;
        }

        int i = v1 - 'A';
        int j = v2 - 'A';

        grafo[i][j].aresta = true;
        grafo[j][i].aresta = true; 
        grafo[i][j].peso = peso;
        grafo[j][i].peso = peso;
        maxvert -= 1;
    }
}

void DFS_NDP(Vertice_NDP **grafo,int vert){
    cout << "Digite o Vertice_NDPs que deseja começar: " << endl;
    char Vertice_NDP;
    cin >> Vertice_NDP;
}

void ImprimirGrafo_NDP(Vertice_NDP **grafo, int vert){
    char letra = 'A';   
    cout << "  ";
    for (int i = 0; i < vert; i++) {
        cout << letra << "      ";
        letra += 1;     
    }
    cout << endl;
    letra = 'A';
    for(int i=0;i<vert;i++){
        cout << letra << " ";
        for(int j=0;j<vert;j++){
            cout << grafo[i][j].aresta << " P:" << grafo[i][j].peso << "| ";
        }
        cout << endl;
        letra += 1;
    }
}