#include <iostream>
#include <vector>
using namespace std;

template<typename type>
vector <type> insertion_sort(vector <type> mass) { // функция, которая возвращает отсортированный динамический массив
    unsigned int mass_length = mass.size(); type memory; vector <type> result_mass(mass);

    for (unsigned int i = 0; i < mass_length; ++i) {
        unsigned int current = i;
        while ((current != 0) && (result_mass[current] < result_mass[current - 1])) {
            memory = result_mass[current - 1];
            result_mass[current - 1] = result_mass[current];
            result_mass[current] = memory;
            current--;
        }
    }

    return result_mass;
}


int main()
{
    unsigned int n, m;
    cin >> n; vector <int> mass(n);
    for (unsigned int i = 0; i < n; ++i) {
        cin >> mass[i];
    }

    vector <int> sorted_mass = insertion_sort(mass);

    for (unsigned int i = 0; i < sorted_mass.size(); ++i) {
        cout << sorted_mass[i] << " ";
    }
}