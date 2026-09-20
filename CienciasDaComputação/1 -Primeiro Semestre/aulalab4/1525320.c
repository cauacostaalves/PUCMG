#include <stdio.h>
int main() {
  int lado, linha, coluna, diagonal1, diagonal2;
  char quadrado;
  printf("Tamanho do lado:");
  scanf("%d%*c", &lado);
  printf("Escolha sua figura:");
  scanf("%c%*c", &quadrado);
  diagonal1 = 1;
  diagonal2 = lado - 2;
  switch (quadrado) {

  case 'c': // quadrado preenchido
    for (linha = 0; linha < lado; linha++) {
      for (coluna = 0; coluna < lado; coluna++) {
        printf("*");
      }
      printf("\n");
    }
    break;

  case 'b': // somente bordas
    for (linha = 0; linha < lado; linha++) {
      for (coluna = 0; coluna < lado; coluna++) {
        if (coluna == 0 || coluna == lado - 1 || linha == 0 ||
            linha == lado - 1) {
          printf("*");
        } else {
          printf(" ");
        }
      }
      printf("\n");
    }
    break;

  case 'p': // borda e diagonal principal
    for (linha = 0; linha < lado; linha++) {
      for (coluna = 0; coluna < lado; coluna++) {
        if (linha == 0 || coluna == 0 || coluna == lado - 1 ||
            linha == lado - 1) {
          printf("*");
        } else if (linha == diagonal1 && coluna == diagonal2) {
          printf("*");
          diagonal1++;
          diagonal2--;
        } else {
          printf(" ");
        }
      }
      printf("\n");
    }
    break;
  case 's': // borda e diagonal secundaria
    for (linha = 0; linha < lado; linha++) {
      for (coluna = 0; coluna < lado; coluna++) {

        if (coluna == 0 || coluna == lado - 1 || linha == 0 ||
            linha == lado - 1 || linha == coluna) {
          printf("*");
        } else {
          printf(" ");
        }
      }
      printf("\n");
    }
    break;
  case 'h': // bordas e divisao horizontal(meio)
    for (linha = 0; linha < lado; linha++) {
      for (coluna = 0; coluna < lado; coluna++) {

        if (coluna == 0 || coluna == lado - 1 || linha == 0 ||
            linha == lado - 1 || coluna == lado / 2) {
          printf("*");
        } else {
          printf(" ");
        }
      }
      printf("\n");
    }
    break;

  case 'v': // bordas e divisao vertical(meio)
    for (linha = 0; linha < lado; linha++) {
      for (coluna = 0; coluna < lado; coluna++) {

        if (coluna == 0 || coluna == lado - 1 || linha == 0 ||
            linha == lado - 1 || linha == lado / 2) {
          printf("*");
        } else {
          printf(" ");
        }
      }
      printf("\n");
    }
    break;

  default:
    printf("Não existe essa opção");
  }

  return 0;
}
