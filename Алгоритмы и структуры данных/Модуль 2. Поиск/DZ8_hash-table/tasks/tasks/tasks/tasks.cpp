// В этом файле хранятся алгоритмические решения задач из 8 дз.
// Поскольку я не придумал (или не посчитал необходимым) использовать для данных задач хеш-функции,
// я реализовал решения функционально
// Тем не менее шаблон хеш-таблицы можно найти в папке DZ8_hash_table/hash_table
// Задачи:
// 1) На вход функции подается две строки a и b
//    Используя хеш таблицу, попытайтесь определить, является ли строка b анаграммой к строке a.
// 2) Есть строка с многократным повторением разных букв. Надо понять какое максимальное количество повторений встречается в строке. 
//    Используя хеш таблицу, записывайте пару буква - количество вхождений в строку.


#include <iostream>
#include <map>

const unsigned int start_with = 97;

void task1() {
    std::map<char, int> Map1, Map2;
    //for (unsigned int i = 0; i < 26; ++i) { оказывается предзаполнение ненужно Map[i]++ в случае, если ключ i раньше не встречалался, Map[i] считается равным нулю
    //                                        следовательно при выполнении Map[i]++ в Map[i] запишется 1
    //    Map1[start_with + i] = 0;
    //    Map2[start_with + i] = 0;
    //}

    std::string str1, str2;
    std::cin >> str1 >> str2;

    for (char elem : str1) {
        Map1[elem]++;
    }

    for (char elem : str2) {
        Map2[elem]++;
    }

    if (Map1 == Map2) { std::cout << "true"; }
    else { std::cout << "false"; }
}


void task2() {
    std::map<char, int> Map;
    //for (unsigned int i = 0; i < 26; ++i) { см.выше
    //    Map[start_with + i] = 0;
    //}

    std::string str;
    std::cin >> str;

    for (char elem : str) {
        Map[elem]++;
    }

    int maxim = -1;
    for (char elem : str) {
        if (Map[elem] > maxim) { maxim = Map[elem]; };
    }

    std::cout << maxim;
}

int main()
{
    task1();
    task2();
}