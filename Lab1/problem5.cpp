#include <iostream>
using namespace std;

int main() {
    int f_dig = 0, s_dig = 1;
    int digit;
    cin >> digit;
    int counter = 3;
    int tmp;
    while (f_dig + s_dig <= digit) {
        counter++;
        tmp = s_dig;
        s_dig += f_dig;
        f_dig = tmp;
    }
    cout << f_dig + s_dig << endl;
    cout << "Number: " << counter << endl;
}