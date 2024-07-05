// Дан не отсортированный массив целых чисел.Необходимо перенести в начало массива все четные числа.При этом последовательность четных чисел должна быть сохранена.
// То есть если 8 стояла после 6, то после переноса в начало, она по - прежнему должна стоять после 6.

#include <iostream>
#include <vector>
using namespace std;

int main()
{   
    // иницилизация начальных значений
    unsigned int n; cin >> n;
    vector <int> mass(n);
    
    for (unsigned int i = 0; i < n; ++i) {
        cin >> mass[i];
    }


    // решение

    unsigned int i = 0, j, memory;

    while (mass[i] % 2 == 0) { i++; }; j = i + 1;

    for (; j < n; ++j) {
        if (mass[j] % 2 == 0) {
            memory = mass[i];
            mass[i] = mass[j];
            mass[j] = memory;
            i++;
        }
        while (mass[i] % 2 == 0) { i++; }
    }
    
    for (i = 0; i < n; i++) {
        cout << mass[i] << " ";
    }
}

