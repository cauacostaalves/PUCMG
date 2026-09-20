#include <stdio.h>

int main(){
   int lado,d1,d2;
   char op;
   scanf("%d%*c",&lado);
 scanf("%c%*c",&op);
   switch(op){
   case 'c': //imprima um quadrado totalmente preenchido;
    for(int l=0;l<lado;l++){
        for(int c=0;c<lado;c++){
            printf("*");
        }printf("\n");
    }
   break;

   case 'b'://imprima somente suas bordas;
         for(int l=0;l<lado;l++){
        for(int c=0;c<lado;c++){
            if(c == 0 || c == lado-1 || l == 0 ||
            l == lado-1){
                    printf("*");
            }else{
            printf(" ");
            }
        }printf("\n");
    }

   break;

   case 'p'://imprima suas bordas e a diagonal principal;
    for(int l=0;l<lado;l++){
        for(int c=0;c<lado;c++){
            if(c == 0 || c == lado-1 || l == 0 ||
            l == lado-1||l==c){
                    printf("*");
            }else{
            printf(" ");
            }
        }printf("\n");
    }

   break;

   case 's':// imprima suas bordas e a diagonal secundária;
       d1=1;
       d2=lado-2;
 for(int l=0;l<lado;l++){
        for(int c=0;c<lado;c++){
            if(c == 0 || c == lado-1 || l == 0 ||
            l == lado-1){
                    printf("*");
            }
            else if(l==d1&&c==d2){
                    printf("*");
                    d1++;
                    d2--;
                    }
            else{
            printf(" ");
            }
            if(c==d1 && l==d2){
                printf("*");
                d1--;
                d2++;
            }

        }printf("\n");
    }

   break;

   case 'h':// imprima suas bordas e sua divisão horizontal (no meio);
for(int l=0;l<lado;l++){
        for(int c=0;c<lado;c++){
            if(c == 0 || c == lado-1 || l == 0 || l == lado-1|| l==lado/2 ){
                    printf("*");
            }else{
            printf(" ");
            }
        }printf("\n");
    }
   break;

   case 'v'://imprima suas bordas e sua divisão vertical (no meio);
for(int l=0;l<lado;l++){
        for(int c=0;c<lado;c++){
            if(c == 0 || c == lado-1 || l == 0 ||
            l == lado-1 || c==lado/2){
                    printf("*");
            }else{
            printf(" ");
            }
        }printf("\n");
    }
   break;
   default:
   printf("Não existe essa opção");
   break;
   }


return 0;
}
