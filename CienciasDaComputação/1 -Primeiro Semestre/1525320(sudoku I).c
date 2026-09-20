#include <stdio.h>

//case 1 em funçao

int m[9][9];
void ler_matriz() // le a matriz
{
    for(int l=0; l<9; l++)
    {
        for(int c=0; c<9; c++)
        {
            scanf("%d%*c",&m[l][c]);
        }
    }
}

void case1()
{
    int lin, col, alg;
    scanf("%d%*c",&lin); //escolhe a linha de 0 a 8
    scanf("%d%*c",&col); //escolhe a coluna de 1 a 9
    scanf("%d%*c",&alg); //escolhe o algarismo de 1 a 9
    m[lin][col]=alg;  // faz com que a escolha do usuario se encaixe na nova matriz
}

void case2()
{
    int rep, col,lin, cont;
    lin=0;
    rep=0;
    col=0;
    scanf("%d%*c",&lin);// o usuario escolhe uma linha a ser analisada
    for(int l=0; l<9; l++)
    {
        cont=0;
        for (int c=0; c<9; c++)
        {
            if(m[lin][col]==m[lin][c]) // esse if faz aumentar o cont sempre que achar algum numero igual da mesma linha
            {
                cont++;
            }
            if(cont>1) // se achar entao aumenta-se um no rep para que possa imprimir S ou N
            {
                rep++;
            }
        }

        col++;
    }

    if(rep>1)
    {
        printf("S\n");
    }
    else
    {
        printf("N\n");
    }
}
void case3()
{
    int lin, rep, col,cont;
    lin=0;
    rep=0;
    col=0;
    scanf("%d%*c",&col);  // o usuario escolhe uma coluna a ser analisada
    for(int l=0; l<9; l++)
    {
        cont=0;
        for(int c=0; c<9; c++)
        {
            if( m[lin][col] == m[c][col]) // esse if faz aumentar o cont sempre que achar algum numero igual da mesma coluna
            {
                cont++;
            }
            if(cont>1)  // se achar entao aumenta-se um no rep para que possa imprimir S ou N
            {
                rep++;
            }
        }
        lin++;
    }

    if(rep>0)
    {
        printf("S\n");
    }
    else
    {
        printf("N\n");
    }
}
void case4()
{
    int quad, lin, col, rep, cont;
    scanf("%d%*c",&quad); // escolhe o quadrante a ser analizado
    //logica para pegar as linhas de cada quadrante
    lin=3*(quad/3);  // com essa logica matematica se sabe a linha de acordo com o quadrante escolhido
    col=3*(quad%3);// com essa logica matematica se sabe a coluna de acordo com o quadrante escolhido
    rep=0;
    for(int l=lin; l<lin+3; l++)
    {
        for(int c=col; c<col+3; c++)
        {
            cont=0;
            for(int li=lin; li<lin+3; li++)
            {
                for(int co=col; co<col+3; co++)
                {
                    if(m[l][c]==m[li][co] && (l!=li || c!=co)) //comparando as matrizes se for igual e de linha e coluna diferente
                    {
                        cont++;
                    }
                }
            }
            if(cont>0)
            {
                rep++;
            }
        }
    }
    if(rep>0)
    {
        printf("S\n");
    }
    else
    {
        printf("N\n");
    }
}
void case5()
{
    int rep;
  rep=0;
            for(int l=0; l<9; l++)  // nesse primeiro comando le-se todas linhas da matriz
            {

                for (int c=0; c<9; c++)
                {
                    for(int co=c+1; co<9; co++)
                    {
                        if(m[l][c]==m[l][co]) // verefica se repettiu algum numero nas linhas
                        {
                            rep=1;
                        }
                    }
                }
            }

            int rep1=0;
            for(int l=0; l<9; l++)  // lendo coluna pra ve se repete
            {

                for (int c=0; c<9; c++)
                {
                    for(int lin=l+1; lin<9; lin++)
                    {
                        if(m[l][c]==m[lin][c])
                        {
                            rep1=1;
                        }
                    }
                }
            }

            int rep2=0;
            int linha=0;
            int coluna=0;
             do {
        for (int l = linha; l < linha + 3; l++) { // com essa logica estamos comparando todos os quadrantes da matriz
            for (int c = 0; c < 9; c++) {
                for (int lin = linha; lin < linha + 3; lin++) {
                    for (int col = 0; col < 9; col++) {
                        if (l != lin || c != col) { // Evita comparar o mesmo elemento
                            if (m[l][c] != 0 && m[l][c] == m[lin][col]) {
                                rep2 = 1;
                            }
                        }
                    }
                }
            }
        }
        linha = linha + 3;
    } while (linha < 9);

            if(rep>0 && rep1>0 && rep2>0) //sim tem perdeu o sudoku
            {
                printf("S\n");
            }
            else
            {
                printf("N\n"); // ganhou o sudoku
            }

}
void case_default()
{
    for(int l=0; l<9; l++)
    {
        if(l==3 || l==6)
        {
            printf("\n");
        }
        for(int c=0; c<9; c++)
        {
            if(c==3 || c==6)
            {
                printf("  ");
            }
            printf("%d ",m[l][c]);
        }
        printf("\n");
    }
}
int main()

{
    int op,cont=0,rep=0,quad,lin,col,alg;
    ler_matriz();
    do
    {
        scanf("%d%*c",&op);
        switch(op)
        {
        case 0:
            //finaliza o programa
            break;
        case 1:    //1. Atualizar �ndice: Juntamente com a op��o 1, na mesma linha, ser�o apresentadas o �ndice da linha (entre 0 a 8),
            // o �ndice da coluna (0 a 8), e o novo algarismo (1 a 9) que ir� substituir o algarismo antigo na posi��o indicada.
            case1();
            break;

        case 2: //2. Verificar linha: Juntamente com a op��o 2, na mesma linha, ser� apresentada o �ndice da linha (entre 0 a 8) a ser verificada.
            //Se n�o houver repeti��o de n�meros, deve-se imprimir o caractere 'N'; caso contr�rio, imprima 'S'.
            case2();

            break;

        case 3:  //3. Verificar coluna: Juntamente com a op��o 3, na mesma linha, ser� apresentada o �ndice da coluna (entre 0 a 8) a ser verificada.
            // Se n�o houver repeti��o de n�meros, deve-se imprimir o caractere 'N'; caso contr�rio, imprima 'S'.
            case3();
            break;

        case 4:     //4. Verificar quadrante: Juntamente com a op��o 4, na mesma linha, ser� apresentada o �ndice do quadrante (0 a 8) a ser verificado.
            //Se n�o houver repeti��o de n�meros, deve-se imprimir o caractere 'N'; caso contr�rio, imprima 'S'. Os quadrantes est�o enumerados na seguinte forma:
            //  0 1 2
            //  3 4 5
            //  6 7 8
            case4();

            break;

        case 5: //5. Verificar matriz: Se n�o houver repeti��o de n�meros, deve-se imprimir o caractere 'N';
            //caso contr�rio, imprima 'S'
            case5();
            break;
        default:  /*Qualquer outro valor que n�o esteja no intervalo [0,5], assume-se que o usu�rio deseja imprimir a matriz de maneira formatada,
                   deixando claro a separa��o dos quadrantes via espa�os e linhas em branco. */
            case_default();
            break;
        }
    }
    while(op!=0);

    return 0;
}
