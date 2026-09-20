#include <stdio.h>

int main()
{
    int nota;
    char res;
    scanf("%d%*c",&nota); //lendo a nota do usuario

    res = 'N'+ 5*(nota/60);//imprime se o usario foi aprovado +60 nota
    printf("%c\n",res);

    res= 'N'+ 5*(nota/45)*(1-(nota/60)); //se nota for entre 59 e 45 recuperaçao
    printf("%c\n",res);

    res='N'+ 5* (1-(nota/45))*(1-(nota/60)); // nota abaixo de 45 reprovado >45 print S <45 N
    printf("%c\n",res);

    return 0;
}
