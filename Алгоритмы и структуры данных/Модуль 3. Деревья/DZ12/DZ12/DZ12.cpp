// Дана последовательность целых чисел и число K. Найдите максимальное произведение подпоследовательности размером K.

#include <iostream>
#include <vector>

int main()
{
    int n, k; std::cin >> n; std::vector<int> mass(n);

    for (int i = 0; i < n; ++i)
        std::cin >> mass[i];

    std::cin >> k;

    int cur = 1;
    for (int j = 0; j < k; ++j)
        cur *= mass[j];

    int maxim = cur;
    for (int j = k; j < n; ++j) {
        if (mass[j] != 0) {
            cur *= mass[j];
            cur /= mass[j - k];
        }
        else { // обработка нуля
            if (maxim < 0)
                maxim = 0;

            cur = 1;
            if (j + k < mass.size())
                for (int t = j + 1; t <= j + k; ++t)
                    cur *= mass[t];
            else
                cur = 0;

            j += k;
        }

        if(cur > maxim)
            maxim = cur;
    }

    std::cout << maxim;
}
