#include <stdio.h>

int main()
{
    int num[10],op,new_num,ind,acc=0,cont=0,par,impar;
    int max,min,ordem,original[10],repetiu;
    float med;
    for(int i=0; i<10; i++)
    {
        scanf("%d%*c",&num[i]);
        original[i]=num[i];
    }
    do
    {
        scanf("%d%*c",&op);
        switch(op)
        {
        case 0: // 0. Sair do programa
            break;
        case 1: // 1. Atualizar o valor em um índice específico (fornecido pelo(a) usuário(a)) da sequência
            scanf("%d%*c",&ind);
            scanf("%d%*c",&new_num);
            num[ind]=new_num;
            break;
        case 2: //2. Exibir sequência
            for(int i=0; i<10; i++)
            {
                printf("%d ",num[i]);
            }
            break;
        case 3: //3. Exibir sequência em ordem reversa
            for(int i=9; i>=0; i--)
            {
                printf("%d ",num[i]);
            }
            break;
        case 4: // 4. Exibir a somatória de todos os valores da sequência
            acc=0;
            for(int i=0; i<10; i++)
            {
                acc+=num[i];
            }
            printf("%d\n",acc);
            break;
        case 5: // 5. Exibir a contagem de números duplicados da sequência
      acc = 0;
      for (int i = 0; i < 10; i++) {
        int repetiu = 0;
        for (int j = 10; j > i; j--) {

          if (num[i] == num[j]) {
            acc++;
            repetiu++;
          }
        }
        if (repetiu >= 2) {
          acc -= (repetiu - 1);
        } else if (repetiu == 1) {
          acc += 1;
        } else {
          acc = acc;
        }
      }

      printf("%d", acc);

      break;

        case 6: // 6. Exibir os valores únicos (i.e., sem repetição) da sequência

      for (int i = 0; i < 10; i++) {
        acc = 0;

        for (int j = 10; j > i; j--) {

          if (num[i] == num[j]) {
            acc++;
          }
        }
        for (int l = 0; l < 10; l++) {

          if (num[i] == num[l] && i != l) {
            acc++;
          }
        }
        if (acc == 0) {

          printf("%d ", num[i]);
        }
      }
            break;
        case 7: // 7. Exibir a frequência de todos os valores da sequência
            break;
             for (int i = 0; i < 10; i++) {
        acc = 0;
        int acc1 = 0;

        for (int j = 10; j > i; j--) {

          if (num[i] == num[j]) {
            acc++;
          }
        }
        for (int l = 0; l < 10; l++) {

          if (num[i] == num[l] && i != l) {
            acc1++;
          }
        }

        if (acc == 0 && acc1 == 0) {
          printf("%d: 1 vez  ", num[i]);
        }

        else if (acc == 0 && acc1 > 0) {

          printf("%d:%d vezes  ", num[i], (acc1 + 1));
        }
      }
        case 8: // 8. Exibir o valor máximo, mínimo e médio (com duas casas decimais) da sequência
            max=num[0];
            min=num[0];
            acc=0;
            for(int i=0; i<10; i++)
            {
                if(num[i]>max)
                {
                    max=num[i];
                }
                if(num[i]<min)
                {
                    min=num[i];
                }
                acc+=num[i];

            }
            med=(float) acc/10;
            printf("%d ",min);
            printf("%d ",max);
            printf("%0.2f\n",med);
            break;
        case 9 : // 9. Exibir os valores pares e ímpares da sequência (sem repetição)
            for(int i=0; i<10; i++)
            {
                for(int j=i+1; j<10; j++)
                {
                    if(num[i]>num[j])
                    {
                        ordem=num[i];
                        num[i]=num[j];
                        num[j]=ordem;
                    }
                }
            }
            printf("P: ");
            for(int i=0; i<10; i++)
            {
                for(int i=0; i<10; i++)
                {
                    for(int j=i+1; j<10; j++)
                    {
                        if(num[i]>num[j])
                        {
                            ordem=num[i];
                            num[i]=num[j];
                            num[j]=ordem;
                        }
                    }
                }
                if(num[i]%2==0 )
                {
                    par=num[i];
                    printf("%d ",par);
                }
            }
            printf("I: ");
            for(int i=0; i<10; i++)
            {
                if(num[i]%2==1 )
                {
                    impar=num[i];
                    printf("%d ",impar);
                }
            }
            for(int i=0; i<10; i++)
            {
                num[i]=original[i];
            }
            break;
        case 10: // 10. Exibir a sequência rotacionada M vezes (quantidade fornecida pelo(a) usuário(a)) para a esquerda
            break;
        case 11: // 11. Exibir a sequência rotacionada M vezes (quantidade fornecida pelo(a) usuário(a)) para a direita
            break;
        }
    }
    while(op!=0);





    return 0;
}
