// Представьте, что у вас есть электронная библиотека, содержащая информацию о книгах. Каждая книга имеет уникальный номер ISBN, название и год издания.
//Вам нужно написать программу, которая сортирует все книги по году издания в порядке возрастания.Если две или более книг были изданы в один и тот же год, 
//сортируйте их по названию в алфавитном порядке.Используйте сортировку слиянием для решения этой задачи.


#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <Windows.h>    
using namespace std;

struct book {
    unsigned int ISMN_number;
    unsigned short year;
    string name;
};

struct book_nest {
    shared_ptr<book_nest> next;
    unsigned short year;
    vector<book> books;
};

void insert_nest(book inserting_book, shared_ptr<book_nest> insert_where);

vector <book> merge_sort(vector <book> mass); // реализация (была сделана пара изменений) взята из другой задачи, оригинал хранится в папке sorts



// область объявления глоабльных перменных
shared_ptr<book_nest> first = nullptr;
int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    // инициализация переменных
    shared_ptr<book_nest> current;

    unsigned int n; cin >> n; vector<book> mass_books (n);
    string str; string memory; book memory_book;

    // считываем очередной элемент

    for (unsigned int i = 0; i < n; ++i) {
        memory = ""; // обнуляем поле
        cin >> str;

        memory_book.ISMN_number = std::stoull(str); // считываем ISMN-номер

        while (str[str.size() - 1] != '\"') { // считываем название книги
            cin >> str;
            memory += str + " ";
        }
        memory_book.name = memory.substr(0, memory.size() - 1);

        cin >> str; // считываем год книги
        memory_book.year = std::stoi(str);

        // записываем элемент в память

        current = first;
        bool is_inserted = false;
        if (current == nullptr) { // если список пустой - добавляем первое гнездо
            insert_nest(memory_book, current);
            is_inserted = true;
        }else if (current->year > memory_book.year) { // если год первого гнезда больше, то создаем новое гнездо перед первым
            insert_nest(memory_book, current);
            is_inserted = true;
        }

        while (!is_inserted) {
            if (current->year == memory_book.year) { // если год текущего гнезда равен году считанной книги, то заносим книгу в массив
                current->books.push_back(memory_book);
                is_inserted = true;
            }
            else if (current->year < memory_book.year) { // ((current->next = nullptr) || (current->next->year > memory_book.year))
                                                         // если год текущего гнезда меньше года считанной книги и год следующего гнезда
                                                         // больше года считанной книги или следующего гнезда нет (т.е. текущее гнездо - последнее в списке)
                if (current->next == nullptr) {          
                    insert_nest(memory_book, current); 
                    is_inserted = true;
                }
                else if (current->next->year > memory_book.year) {
                    insert_nest(memory_book, current);
                    is_inserted = true;
                }
            }
            current = current->next;
        }
    }

    // сортировка элементов в каждом гнезде

    current = first; 
    while (current->next != nullptr) {
        current->books = merge_sort(current->books);
        current = current->next;
    }
    current->books = merge_sort(current->books);

    // вывод в нужном формате
    
    current = first;
    while (current->next != nullptr) {
        for (int i = 0; i < current->books.size(); ++i) {
            cout << current->books[i].ISMN_number << " " << current->books[i].name << " " << current->books[i].year << endl;
        }
        current = current->next;
    }
    for (int i = 0; i < current->books.size(); ++i) {
        cout << current->books[i].ISMN_number << " " << current->books[i].name << " " << current->books[i].year << endl;
    }
}

void insert_nest(book inserting_book, shared_ptr<book_nest> insert_where) {
    shared_ptr<book_nest> pointer (new book_nest);

    pointer->year = inserting_book.year; pointer->books.push_back(inserting_book);
    if (insert_where == nullptr) { // если список пустой - создаем первое гнездо в нем
        pointer->next = nullptr;
        ::first = pointer;
    }
    else if (insert_where == first) { // если необходимо вставить гнездо в начало списка
        if (insert_where->year > pointer->year) { // если необходимо вставить перед первым элементом 
            pointer->next = ::first;
            ::first = pointer;
        }
        else { // если необходимо вставить после первого элемента (insert->where->year <= pointer->year
            pointer->next = insert_where->next;
            insert_where->next = pointer;
        }
    }
    else if (insert_where->next == nullptr) { // если необходимо вставить гнездо в конец списка
        insert_where->next = pointer;
        pointer->next = nullptr;
    }
    else { // если необходимо вставить гнездо посередине списка
        pointer->next = insert_where->next;
        insert_where->next = pointer;
    }
}

vector <book> join_massives(vector <book> mass1, vector <book> mass2) { // функция, которая объединяет два отсортированных массива в один отсортированный
    vector <book> result_mass(mass1.size() + mass2.size()); unsigned int i = 0, index1 = 0, index2 = 0;

    while ((mass1.size() > index1) && (mass2.size() > index2)) { // Цикл работает пока один из индексов не вылезет за пределы массива
        if (mass1[index1].name < mass2[index2].name) { result_mass[i] = mass1[index1]; index1++; }
        else { result_mass[i] = mass2[index2]; index2++; }
        ++i;
    }
    // Дозаполняем массив
    while (mass1.size() > index1) { result_mass[i] = mass1[index1]; index1++; ++i; }   // Если в результате работы предыдущего цикла index2 вылез за пределы, то этот цикл дозаполняет массив элементами из первого массива
    while (mass2.size() > index2) { result_mass[i] = mass2[index2]; index2++; ++i; }   // Если в результате работы предыдущего цикла index1 вылез за пределы, то этот цикл дозаполняет массив элементами из второго массива

    return result_mass;
}

vector <book> merge_sort(vector <book> mass) { // функция, которая возвращает отсортированный динамический массив. Восходящая реализация
    unsigned int mass_length = mass.size(); unsigned int middle = mass_length / 2;
    vector <book> result_mass(mass); vector <book> mass1; vector <book> mass2;

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