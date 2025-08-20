#include <stdio.h>

int main(){
    int A = 0, B = 0, C = 0, D = 0;
    scanf("%d %d %d %d", &A, &B, &C, &D);
    int DIFERENCA = (A * B - C * D);
    printf("DIFERENCA = %d\n", DIFERENCA);
    return 0;
}