#include <iostream>
#include <stack>
#include <string>
using namespace std;

int main() {
    stack <char> S;
    string str;
    char sym;
    cin >> str;
    while(str.length() != 0) {
        sym = str.back();
        str.pop_back();
        if (sym == ')' || sym == ']') {
            S.push(sym);
        }
        else if (sym == '(') {
            if (S.empty()) {
                cout << "No" << endl;
                return 0;
            }
            else if (S.top() == ')') {
                S.pop();
            }
            else {
                cout << "No" << endl;
                return 0;
            }
        }
        else if (sym == '[') {
            if (S.empty()) {
                cout << "No" << endl;
                return 0;
            }
            else if (S.top() == ']') {
                S.pop();
            }
            else {
                cout << "No" << endl;
                return 0;
            }
        }
    }
    if (S.empty()) cout << "Yes" << endl;
    else cout << "No" << endl;
}