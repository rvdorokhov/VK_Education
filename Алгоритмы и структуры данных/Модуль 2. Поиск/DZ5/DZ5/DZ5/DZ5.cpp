#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <stack>
#include <cstring>

using namespace std;

int main()
{
    string str, result;
    deque <char> S; char letter;

    cin >> str;

    S.push_back(str[0]);
    for (unsigned int i = 1; i < str.size(); ++i) {
        letter = str[i];
        if (letter == S.back()) {
            S.pop_back();
        }
        else {
            S.push_back(letter);
        }
    }

    unsigned int n = S.size(); str = "";
    for (unsigned int i = 0; i < n; ++i) {
        str += S.front();
        S.pop_front();
    }

    //char* str1 = new char[str.size()];
    //strcpy(str1, str.c_str());

    cout << str.c_str() << endl;
}
