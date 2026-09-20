#include <stdio.h>

int main()
{
    int num[10],op,indice,i,sequencia;

    for( i=0; i<10; i++) //Lendo meus numeros
    {
        printf("Digite seu numero\n");
        scanf("%d",&num[i]);
    }
    printf("Escolha sua opcao: \n ");
    scanf("%d%*c",&op);
    switch(op)
    {
    case 1: // Atualizar o valor em um �ndice espec�fico (fornecido pelo(a) usu�rio(a) da sequ�ncia

        break;
    case 2: //Exibir sequencia
        printf("Sua sequencia e:");
        for(i=0; i<10; i++)
        {
            printf("%d ",num[i]);
        }

        break;
    case 3: //Exibir sequencia inversa
        printf("Sua sequencia e:");
        for(i=9; i>=0; i--)
        {
            printf("%d ",num[i]);
        }

        break;
    case 4: //Exibir a somatoria
        printf("Sua sequencia e:");
        for(i=10; i>0; i--)
        {
            printf("%d ",num[i]);
        }

        break;



    }



    return 0;
}
