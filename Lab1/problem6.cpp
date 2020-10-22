#include <iostream>
#include <cmath>
using namespace std;

int main() {
    int digit;
    float tmp;
    cin >> digit;
    int i = 1;
    while (i * i < digit / 2) {
        tmp = sqrt(digit - i * i);
        if (tmp == int(tmp)) {
            cout << i * i << " + " << int(tmp) * int(tmp) << endl;
        }
        i++;
    }
}