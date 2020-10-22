#include <iostream>
using namespace std;

int main() {
    int digit;
    cin >> digit;
    int sum = 0;
    for (int i = 1; i < digit; i++) {
        for (int j = 0; j < i; j++) {
            sum++;
        }
    }
    sum = sum * 2 + 1;
    int sum2 = 0;
    cout << digit << "^3 = ";
    for (int i = 0; i < digit; i++) {
        cout << sum;
        if (i + 1 < digit) cout << " + ";
        sum2 += sum;
        sum += 2;
    }
    cout << " = " << sum2 << endl;
}