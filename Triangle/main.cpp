/**
 * This program outputs a triangle.
 * ntype=1 for a star filled  triangle;
 * ntype=2 for a number surrounded triangle.
 */

#include <iostream>
#include <cmath>
#include "console.h"
using namespace std;

int printTriangle(int layer);
int printNumTriangle(int layer);
int cppVersion();

int main() {
    cout << "The Current Cpp Version:" << __cplusplus << endl;
    int layer, ntype;
    cout << "Select the Type:";
    cin >> ntype;
    cout << "Input the layer of the Triangle:";
    cin >> layer;
    if(ntype == 1){
        printTriangle(layer);
    }
    else if(ntype == 2){
        printNumTriangle(layer);
    }
    return 0;
}


int printTriangle(int layer){
    if(layer == 0){
        layer = 6;
    }
    for(int i = layer-1; i >= 0; i--){
        for(int j = -layer; j<=layer; j++){
            int sum = abs(i) + abs(j);
            if((sum + 1)% 2 == layer % 2 && sum <= layer - 1 ){
                cout <<'*';
            }
            else{
                cout <<' ';
            }
            }
        cout<<endl;
        }

    return 0;
    }

int printNumTriangle(int layer){
    if(layer == 0){
        layer = 6;
    }
    for(int i = layer - 1; i >= 0; i--){
        for(int j = -layer; j<=layer; j++){
            int sum = abs(i) + abs(j);
            if(sum == layer - 1 ){
                cout << layer - i;
            }
            else if((sum + 1)% 2 == layer % 2 && sum <= layer - 1 && i==0 ){
                cout << layer;
            }
            else {
                cout <<' ';
            }
            }
        cout<<endl;
        }
    return 0;
    }


