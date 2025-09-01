#ifndef GRAFONAODIRECIONADONAOPONDERADO_HPP
#define GRAFONAODIRECIONADONAOPONDERADO_HPP

#include <iostream>

using namespace std;

/* Função para inicializar o grafo com todos pontos com o valor 0 (false) e as colunas
*
*   @param vert Quantidade de vertices do grafo
*   @param grafo O grafo em questão passado
*
*/
void IniciarGrafo_NDNP(bool **grafo, int vert);

/*
* Função para imprimir grafo completo mostrando as ligações de cada aresta 
*
*   @param grafo O grafo em questão passado que vai ser imprimido
*   @param vert Quantidade de vertices do grafo
*   @return o grafo em forma de matriz com 1 representando as arestas
*
*/
void CriarArestas_NDNP(bool **grafo, int vert);

/* Função para imprimir grafo completo mostrando as ligações de cada aresta 
*
*   @param grafo O grafo em questão passado que vai ser imprimido
*   @param vert Quantidade de vertices do grafo
*   @return o grafo em forma de matriz com 1 representando as arestas
*
*/
void ImprimirGrafo_NDNP(bool **grafo,int vert);

/* Função para fazer a busca com profundidade em grafo
*
*   @param grafo O grafo em questão passado que vai ser imprimido
*   @param vert Quantidade de vertices do grafo
*   @return sequencia que achou as vertice
*
*/
void DFS_NDNP(bool **grafo,int vert);


#endif // GRAFONAODIRECIONADONAOPONDERADO_HPP