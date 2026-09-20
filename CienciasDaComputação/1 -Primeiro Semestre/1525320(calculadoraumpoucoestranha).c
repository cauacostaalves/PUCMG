#include <stdio.h>

int main()
{
    int x,y,op,resto=0,acc=1,cont=0,num;
    scanf("%d%*c",&x);
    scanf("%d%*c",&y);
    scanf("%d%*c",&op);

    switch (op)
    {
    case 1:
       num=x;
        do{

        resto=x%10;
        resto=resto*acc;


       // printf("%d ",resto);
        if(cont==y){
num=num-(1000+acc);
printf("%d",num);
        }
        acc=acc*10;
        cont++;
        x=x/10;
      //  printf("%d\n",x);
        }while(x>0);


        break;

    case 2:

        break;

    case 3:
   printf("%d%d",x,y);
        break;
    }


    return 0;
}
