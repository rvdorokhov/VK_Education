// Дан отсортированный по возрастанию массив целых чисел и заданное число.
// Заданное число может и не находиться в массиве.Тогда необходимо вернуть предполагаемый индекс, где мог бы находится элемент.
// Другими словами, найдите правильное место для вставки элемента. Если же число есть, то возвращаем его индекс.

#include <iostream>
#include <vector>

template<typename type>
int bin_search(type search_what, std::vector <type> mass) {
    int left = 0, right = mass.size() - 1, index = mass.size() / 2;
    int result = 0; 
    while ((right >= left) && (!result)) {
        if (search_what == mass[index]) {
            return index;
        }
        else if (search_what > mass[index]) {
            left = index + 1;
        }
        else {
            right = index - 1;
        }
        index = left + (right - left) / 2;
    }

    result = index;

    return result;
}

int main()
{
    int n, search_what;

    std::cin >> n; std::vector<int> mass(n);

    for (int i = 0; i < n; ++i) {
        std::cin >> mass[i];
    }

    std::cin >> search_what;

    std::cout << bin_search(search_what, mass);


}