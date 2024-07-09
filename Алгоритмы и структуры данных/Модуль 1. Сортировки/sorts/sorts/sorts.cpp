#include <iostream>
#include <vector>
#include <string>
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

template<typename type>
vector <type> join_massives(vector <type> mass1, vector <type> mass2) { // функция, которая объединяет два отсортированных массива в один отсортированный
    vector <type> result_mass(mass1.size() + mass2.size()); unsigned int i = 0, index1 = 0, index2 = 0;

    while ((mass1.size() > index1) && (mass2.size() > index2)) { // Цикл работает пока один из индексов не вылезет за пределы массива
        if (mass1[index1] < mass2[index2]) { result_mass[i] = mass1[index1]; index1++; }
        else { result_mass[i] = mass2[index2]; index2++; }
        ++i;
    }
    // Дозаполняем массив
    while (mass1.size() > index1) { result_mass[i] = mass1[index1]; index1++; ++i; }   // Если в результате работы предыдущего цикла index2 вылез за пределы, то этот цикл дозаполняет массив элементами из первого массива
    while (mass2.size() > index2) { result_mass[i] = mass2[index2]; index2++; ++i; }   // Если в результате работы предыдущего цикла index1 вылез за пределы, то этот цикл дозаполняет массив элементами из второго массива

    return result_mass;
}

template<typename type>
vector <type> merge_sort(vector <type> mass) { // функция, которая возвращает отсортированный динамический массив. Восходящая реализация
    unsigned int mass_length = mass.size(); unsigned int middle = mass_length / 2;
    vector <type> result_mass(mass); vector <type> mass1; vector <type> mass2;

    if (mass_length == 1) {
        return mass;
    }
    else {
        for (unsigned int i = 0; i < middle; ++i) { mass1.push_back(mass[i]); } // формируем один массив из элементов первой половины
        for (unsigned int i = middle; i < mass_length; ++i) { mass2.push_back(mass[i]); } // формируем второй массив из элементов второй половины
        mass1 = merge_sort(mass1);
        mass2 = merge_sort(mass2);
        result_mass = join_massives(mass1, mass2);
        return result_mass;
    }
}

int main() {

}