#include <iostream>
#include <vector>
#include <forward_list>

template<
    typename key_t,
    typename value_t
>
class hash_table {
public:
    using element_t = std::pair<key_t, value_t>;
    using bucket_t = std::forward_list<element_t>;

    hash_table() : buckets(117) {} // конструктор без параметров - тогда хеш-таблица по умолчанию создается с размером 117 (число от балды)

    hash_table(unsigned int size) : buckets(size) {} // конструктор с параметром - тогда хеш-таблица создается с размером size

    void insert_elem(element_t element) {
        if (number_of_elements() / buckets.size() >= 0.75) {
            rehash();
        }
        unsigned int index = hash(element.first);
        buckets[index].push_front(element);
    }

    void delete_elem(key_t key) {
        unsigned int index = hash(key);
        buckets[index].remove_if([key](element_t element) { return element.first == key; });
    }

    element_t search_elem(key_t key) {
        element_t result;
        unsigned int index = hash(key);

        for (auto iterator = buckets[index].begin(); iterator != buckets[index].end(); ++iterator) {
            if (iterator->first == key) {
                result.first = iterator->first;
                result.second = iterator->second;
            }
        }

        return result;
    }
private:
    std::vector<bucket_t> buckets;

    float number_of_elements() {
        float counter = 0;
        for (unsigned int i = 0; i < buckets.size(); ++i) {
            for (auto iterator = buckets[i].begin(); iterator != buckets[i].end(); ++iterator) {
                counter++;
            }
        }

        return counter;
    }

    unsigned int hash(key_t key) {
        unsigned int index = std::hash<key_t>()(key);
        return index % buckets.size();
    }

    //unsigned int hash_1(std::string key) {
    //    // Задание с вк - На вход функции подается две строки a и b
    //    //                Используя хеш таблицу, попытайтесь определить, является ли строка b анаграммой к строке a.
    //    // Идея хэш-функции - сумма кодов ascii char-букв строки, поделенная по модулю на size_of_table
    //    unsigned int index = 0;
    //    for (unsigned int i = 0; i < key.size(); ++i) {
    //        index += static_cast<unsigned int>(key[0]); // можно без static_cast, оно автоматически char как целочисденное значение считает
    //    }

    //    return index % buckets.size();
    //}

    //unsigned int hash_2(std::string key) {
    //    // не доделано
    //    // Задание с вк - Есть строка с многократным повторением разных букв. 
    //    //                Надо понять какое максимальное количество повторений встречается в строке.
    //    //                Используя хеш таблицу, записывайте пару буква - количество вхождений в строку.
    //    // Идея хэш-функции - код ascii char-буквы, поделенный по модулю на size_of_table
    //    unsigned int index = static_cast<unsigned int>(key); // можно без static_cast, оно автоматически char как целочисденное значение считает

    //    return index % buckets.size();
    //}

    void rehash() {
        std::vector<bucket_t> old_buckets(buckets.size());
        old_buckets = buckets;
        buckets.clear(); buckets.resize(old_buckets.size() * 3);

        for (unsigned int i = 0; i < old_buckets.size(); ++i) {
            for (auto iterator = old_buckets[i].begin(); iterator != old_buckets[i].end(); ++iterator) {
                insert_elem(*iterator);
            }
        }
    }
};

int main()
{
}
