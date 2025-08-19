#include <iostream>
#include <vector>
using namespace std;

int main() {
    int n = 3; // tamanho da matriz
    vector<vector<int>> grafo(n, vector<int>(n, 0)); 
    // cria uma matriz n x n preenchida com 0

    // adiciona arestas (grafo não direcionado só pra exemplo)
    grafo[0][1] = 1;
    grafo[1][0] = 1;
    grafo[1][2] = 1;
    grafo[2][1] = 1;

    // imprime a matriz
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << grafo[i][j] << " ";
        }
        cout << endl;
    }

    return 0;
}
