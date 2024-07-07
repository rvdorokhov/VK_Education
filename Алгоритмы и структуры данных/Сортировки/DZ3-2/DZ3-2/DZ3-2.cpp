// DZ3-2.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

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
    
    //current = first; // отладочный вывод всех элементов всех гнехж
    //while (current->next != nullptr) {
    //    for (int i = 0; i < current->books.size(); ++i) {
    //        cout << current->books[i].name << " ";
    //    }
    //    cout << endl;
    //    current = current->next;
    //}
    //cout << current->books[current->books.size() - 1].name << endl;
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