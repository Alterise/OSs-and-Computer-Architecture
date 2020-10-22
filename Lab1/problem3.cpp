#include <iostream>
#include <cmath>
using namespace std;

int main() {
    int digit;
    cin >> digit;
    short int a = 1, a0 = a;
    unsigned short b = 1, b0 = a;
    int c = 1, c0 = c;
    unsigned int d = 1, d0 = d;
    long e = 1, e0 = e;

    int i = 1;
    while (i < digit) {
        i++;
        a0 = a0 + pow(i,i);
        if (a + pow(i,i) != a0) {
            cout << i << " - Short Int X_X" << endl;
            break;
        }
        a += pow(i,i);
    }
    if (a == a0) {
        cout << a << endl;
    }

    i = 1;
    while (i < digit) {
        i++;
        b0 = b0 + pow(i,i);
        if (b + pow(i,i) != b0) {
            cout << i << " - Unsigned Short X_X" << endl;
            break;
        }
        b += pow(i,i);
    }
    if (b == b0) {
        cout << b << endl;
    }

    i = 1;
    while (i < digit) {
        i++;
        c0 = c0 + pow(i,i);
        if (c + pow(i,i) != c0) {
            cout << i << " - Int X_X" << endl;
            break;
        }
        c += pow(i,i);
    }
    if (c == c0) {
        cout << c << endl;
    }

    i = 1;
    while (i < digit) {
        i++;
        d0 = d0 + pow(i,i);
        if (d + pow(i,i) != d0) {
            cout << i << " - Unsigned Int X_X" << endl;
            break;
        }
        d += pow(i,i);
    }
    if (d == d0) {
        cout << d << endl;
    }

    i = 1;
    while (i < digit) {
        i++;
        e0 = e0 + pow(i,i);
        if (e + pow(i,i) != e0) {
            cout << i << " - Long X_X" << endl;
            break;
        }
        e += pow(i,i);
    }
    if (e == e0) {
        cout << e << endl;
    }
}