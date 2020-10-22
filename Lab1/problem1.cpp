#include <iostream>
using namespace std;

int main() {
    int a,b;
    cin >> a;
    cin >> b;
    int a1 = a, b1 = b;
    while (a1 != b1) {
        if(a1 > b1) {
            a1 -= b1;
        }
        else {
            b1 -= a1;
        }
    }
    int nod = b1;
    cout << "NOD:" << nod << endl;
    int nok = a * b / nod;
    cout << "NOK:" << nok << endl;
} 