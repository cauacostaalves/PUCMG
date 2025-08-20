#include <stdio.h>
int main(){
    int t = 0;
    int acc = 0;
    for(int i=0; i<4; i++){
        scanf(" %d", &t);
        acc += t;
    }
    printf("%d",acc - 3);
    return 0;
}