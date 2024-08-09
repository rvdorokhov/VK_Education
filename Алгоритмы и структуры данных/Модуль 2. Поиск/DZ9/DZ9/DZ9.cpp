// 1) Дан отсортированный по возрастанию массив и некоторое целое число. 
//    Необходимо найти минимальный подмассив, в котором может содержаться это число методом экспоненциального поиска.
// 2) Дан массив целых чисел. Необходимо вернуть элемент, который встречается больше половины раз. Если такого элемента нет, возвращайте -1

/* 1)   11
        8 11 12 16 18 21 33 36 48 54 63
        16
        Вывод 2 4
        Число 16 находится в подмассиве от индекса 2 до индекса 4
   2)   7
        7 7 8 8 8 8 9
        Вывод
        8
*/

#include <iostream>
#include <map>
#include <set>
#include <vector>

std::pair<int, int> exp_search(int search_what, std::vector<int> mass) {
    int border = 1;

    while (border <= mass.size()) {
        if (mass[border] > search_what) {
            // в "классической" реализации в таком случае запускается бин.поиск от подмассива [border/2; border]
            // но в задании от нас требуется только найти границы подмассива, так что...
            return { border / 2, border };
        }
        border *= 2;
    }

    return { border / 2, mass.size() }; // так же в классической рализации тут должен выполниться бин.поиск
}

void task1() {
    int n, search_what; std::cin >> n; std::vector<int> mass(n);

    for (int i = 0; i < n; ++i) {
        std::cin >> mass[i];
    }

    std::cin >> search_what;

    std::pair<int, int> result = exp_search(search_what, mass);
    std::cout << result.first << " " << result.second;

}

void task2() {
    std::map<int, int> counts;
    std::set<int> keys;
    int n, number;

    std::cin >> n;

    for (int i = 0; i < n; ++i) {
        std::cin >> number;
        counts[number] += 1; // подсчет вхождений

        if (keys.find(number) == keys.end()) // отслеживание ключей
            keys.insert(number);
    }

    int answer = -1;
    for (int key : keys)
        if (counts[key] > n / 2)
            answer = key;

    std::cout << answer;
}

int main()
{
    task1();
}
