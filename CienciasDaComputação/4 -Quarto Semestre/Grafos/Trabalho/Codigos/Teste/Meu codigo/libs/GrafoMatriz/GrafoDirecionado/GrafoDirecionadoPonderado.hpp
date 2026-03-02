#ifndef GRAFODIRECIONADOPONDERADO_HPP
#define GRAFODIRECIONADOPONDERADO_HPP

#include <iostream>

using namespace std;

struct Vertice_DP {
    bool visitado;
    int peso;
    bool aresta;
};

/* Função para inicializar o grafo com todos pontos com o valor 0 (false) e as colunas
*
*   @param vert Quantidade de Vertice_DPs do grafo
*   @param grafo O grafo em questão passado
*
*/
void IniciarGrafo_DP(Vertice_DP **grafo, int vert);

/*
* Função para imprimir grafo completo mostrando as ligações de cada aresta 
*
*   @param grafo O grafo em questão passado que vai ser imprimido
*   @param vert Quantidade de Vertice_DPs do grafo
*   @return o grafo em forma de matriz com 1 representando as arestas
*
*/
void CriarArestas_DP(Vertice_DP **grafo, int vert);

/* Função para imprimir grafo completo mostrando as ligações de cada aresta 
*
*   @param grafo O grafo em questão passado que vai ser imprimido
*   @param vert Quantidade de Vertice_DPs do grafo
*   @return o grafo em forma de matriz com 1 representando as arestas
*
*/
void ImprimirGrafo_DP(Vertice_DP **grafo,int vert);

/* Função para fazer a busca com profundidade em grafo
*
*   @param grafo O grafo em questão passado que vai ser imprimido
*   @param vert Quantidade de Vertice_DPs do grafo
*   @return sequencia que achou as Vertice_DP
*
*/
void DFS_DP(Vertice_DP **grafo,int vert);



#endif // GRAFODIRECIONADOPONDERADO_HPP