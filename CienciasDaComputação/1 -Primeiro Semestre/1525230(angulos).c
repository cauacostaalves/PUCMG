#include <stdio.h>

int main ()
{

    int ang,voltas=0,faltam=0,resto=0;
    scanf("%d%*c",&ang);

    if(ang%90==0)  // se o anggulo se encontra em um dos eixos
    {
        printf("Este angulo se encontra em um dos eixos\n");
    }
    if(ang==0){
        printf("Primeiro quadrante\n");
        printf("0 volta(s) sentido antihorario\n");
        printf("Falta(m) 360 graus (sentido antihorario) para completar 1 volta(s)\n");
    }
    //fala o quadrante dos ang
    if(ang>0)
    {
        resto=ang%360;

        if(resto<=90 && resto>=0 )
        {
            printf("Primeiro quadrante\n");
        }
        else if(resto<=180 && resto>=91 )
        {
            printf("Segundo quadrante\n");
        }
        else if(resto<=270 && resto>=181)
        {
            printf("Terceiro quadrante\n");
        }
        else
        {
            printf("Quarto quadrante\n");
        }
    }
    else if(ang<0){
        ang=ang*(-1);
        resto=ang%360;

        if(resto<=90 && resto>=0 )
        {
            printf("Quarto quadrante\n");
        }
        else if(resto<=180 && resto>=91 )
        {
            printf("Terceiro quadrante\n");
        }
        else if(resto<=270 && resto>=181)
        {
            printf("Segundo quadrante\n");
        }
        else
        {
            printf("Primeiro quadrante\n");
        }
        ang=ang*(-1);
    }
    voltas=ang/360;//le o numero de voltas
    if(ang>0)  // se for positivo
    {
        printf("%d volta(s) sentido antihorario\n",voltas);
    }
    else if(ang<0)   // se negativo
    {
        voltas=voltas*(-1);
        printf("%d volta(s) sentido horario\n",voltas);
    }
    resto=0;
    if(ang>0)  // se o ang é positivo
    {
        faltam=voltas+1; // fala qts voltas ele pode completar
        resto=ang%360; //acha o ang q n sobrou
        resto=360-resto; // e conta qts q falta pra completar 360
        printf("Falta(m) %d graus (sentido antihorario) para completar %d volta(s)\n",resto,faltam);// se o ang é negativo
    }
    else if(ang<0)
    {
        faltam=voltas+1; // fala qts voltas ele pode completar
        ang=ang*(-1); //acha o ang q n sobrou e faz ele ficar positivo
        resto=ang%360; // e conta qts q falta pra completar 360
        resto=360-resto;
        printf("Falta(m) %d graus (sentido horario) para completar %d volta(s)\n",resto,faltam);
    }
    return 0;
}
