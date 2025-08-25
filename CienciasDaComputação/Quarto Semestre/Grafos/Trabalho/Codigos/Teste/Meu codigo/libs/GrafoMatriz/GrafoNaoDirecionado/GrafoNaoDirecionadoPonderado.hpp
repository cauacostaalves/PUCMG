#ifndef GRAFONAODIRECIONADOPONDERADO_HPP
#define GRAFONAODIRECIONADOPONDERADO_HPP

#include <iostream>

using namespace std;

struct Vertice_NDP {
    bool visitado;
    int peso;
    bool aresta;
};

/* Função para inicializar o grafo com todos pontos com o valor 0 (false) e as colunas
*
*   @param vert Quantidade de Vertice_NDPs do grafo
*   @param grafo O grafo em questão passado
*
*/
void IniciarGrafo_NDP(Vertice_NDP **grafo, int vert);

/*
* Função para imprimir grafo completo mostrando as ligações de cada aresta 
*
*   @param grafo O grafo em questão passado que vai ser imprimido
*   @param vert Quantidade de Vertice_NDPs do grafo
*   @return o grafo em forma de matriz com 1 representando as arestas
*
*/
void CriarArestas_NDP(Vertice_NDP **grafo, int vert);

/* Função para imprimir grafo completo mostrando as ligações de cada aresta 
*
*   @param grafo O grafo em questão passado que vai ser imprimido
*   @param vert Quantidade de Vertice_NDPs do grafo
*   @return o grafo em forma de matriz com 1 representando as arestas
*
*/
void ImprimirGrafo_NDP(Vertice_NDP **grafo,int vert);

/* Função para fazer a busca com profundidade em grafo
*
*   @param grafo O grafo em questão passado que vai ser imprimido
*   @param vert Quantidade de Vertice_NDPs do grafo
*   @return sequencia que achou as Vertice_NDP
*
*/
void DFS_NDP(Vertice_NDP **grafo,int vert);



#endif // GRAFONAODIRECIONADOPONDERADO_HPP