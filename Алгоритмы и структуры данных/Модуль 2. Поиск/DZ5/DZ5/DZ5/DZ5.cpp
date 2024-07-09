//Дана строка s.Строка состоит из английских букв в нижнем регистре.Необходимо из строки удалить все рядом стоящие повторяющиеся буквы.
//Например, в строке xyyx мы должны удалить yy, а после и оставшиеся xx и того после должна получиться пустая строка.
//Или же в строке fqffqzzsd после удаления остануться только fsd.Первыми удаляться ff, являющимися третьими и четвертыми символами, затем qq и после уже zz.


#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <stack>
#include <cstring>

using namespace std;

void DZ5_A() {
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

void DZ5_B() {
    stack <int> S;
    unsigned int n, input; cin >> n;
    for (unsigned int i = 0; i < n; ++i) {
        cin >> input;
        S.push(input);
    }

    bool flag = 0;
    while ((S.size() > 0)&&(!flag)) {
        if (S.top() % 2 == 0) {
            cout << S.top();
            flag = 1;
        }
        S.pop();
    }

    if (!flag) { cout << -1 << endl; }
}

int main()
{
    DZ5_B();
}
