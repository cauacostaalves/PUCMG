#include <iostream>
#include "GrafoNaoDirecionadoNaoPonderado.hpp"
using namespace std;

void IniciarGrafo_NDNP(bool **grafo, int vert){
    for(int i=0;i<vert;i++){
        grafo[i] = new bool[vert];
        for(int j=0;j<vert;j++){
            grafo[i][j] = false;
        }
    }
}

void CriarArestas_NDNP(bool **grafo, int vert){
    cout << "Digite os dois vertices que vao ser conectados (Digite 0 0 para sair)." << endl;
    char v1,v2;
    int maxvert = (vert*(vert-1))/2;
    while (maxvert > 0)
    {
        cin >> v1 >> v2;

        if (v1 == '0' && v2 == '0')
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
            cout << "Vertices invalidas! Por favor escreva arestas entre A e " << char('A' + vert - 1) << endl;
            continue;
        }

        int i = v1 - 'A';
        int j = v2 - 'A';

        grafo[i][j] = true;
        grafo[j][i] = true; 
        maxvert -= 1;
    }
}

void DFS_NDNP(bool **grafo,int vert){
    cout << "Digite o vertices que deseja começar: " << endl;
    char vertice;
    cin >> vertice;
    int vert = vertice - 'A';
    Pilha* p;
    IniciarPilha(p);

    for(){
    }
}

void ImprimirGrafo_NDNP(bool **grafo, int vert){
    char letra = 'A';   
    cout << "  ";
    for (int i = 0; i < vert; i++) {
        cout << letra << " ";
        letra += 1;     
    }
    cout << endl;
    letra = 'A';
    for(int i=0;i<vert;i++){
        cout << letra << " ";
        for(int j=0;j<vert;j++){
            cout << grafo[i][j] << " ";
        }
        cout << endl;
        letra += 1;
    }
}