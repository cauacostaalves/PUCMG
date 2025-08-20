#include <iostream>
using namespace std;

int main(){
    int t = 0;
    int acc = 0;
    for(int i=0; i<4; i++){
        cin >> t;
        acc += t;
    }
    cout << acc - 3 << "\n";
}