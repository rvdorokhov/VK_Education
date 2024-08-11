/*
* Решено на основе шаблоны бинарного дерева поиска из папки с шаблонами
* DZ11
* 1) Дано бинарное дерево поиска в виде массива.
     Необходимо найти произведение минимального и максимального значений. Для этого необходимо вспомнить как бинарное дерево размещается в массиве
  2) Дано бинарное дерево поиска в виде массива. Необходимо написать функцию, которая находит и возвращает минимальный элемент в дереве
  DZ13
  3) Дано бинарное дерево поиска в виде массива. Необходимо написать функцию, которая находит и возвращает максимальный элемент в дереве
*/

#include <iostream>
#include <vector>
#include <memory>

class Bin_tree {
protected:
    struct node { // налево идут все вершины <= а направо - все вершины >
        std::shared_ptr<node> parent = nullptr;
        std::shared_ptr<node> left = nullptr;
        std::shared_ptr<node> right = nullptr;
        int key;

        node(int input_key) { key = input_key; }
        node() { }
    };

    std::shared_ptr<node> rootnode = nullptr;
    std::vector<int> result;
public:
    void insert(int key) { // ' ' - мусорное значение, которое подойдет как для числовых, так и для строковых типов данных
        // необходимо для удобного построения дерева без полезной нагрузки
        std::shared_ptr<node> pointer = rootnode;

        if (rootnode == nullptr) {
            rootnode = std::make_shared<node>();
            rootnode->key = key;
        }
        else {
            bool flag = true;
            while (flag) {
                if (key <= pointer->key)
                    if (pointer->left != nullptr)
                        pointer = pointer->left;
                    else {
                        pointer->left = std::make_shared<node>();
                        pointer->left->key = key;
                        pointer->left->parent = pointer;
                        flag = false;
                    }
                else if (key > pointer->key)
                    if (pointer->right != nullptr)
                        pointer = pointer->right;
                    else {
                        pointer->right = std::make_shared<node>();
                        pointer->right->key = key;
                        pointer->right->parent = pointer;
                        flag = false;
                    }
            }
        }
    }

    int get_max() {
        std::shared_ptr<node> pointer = rootnode;
        while (pointer->right != nullptr)
            pointer = pointer->right;

        return pointer->key;
    }

    int get_min() {
        std::shared_ptr<node> pointer = rootnode;
        while (pointer->left != nullptr)
            pointer = pointer->left;

        return pointer->key;
    }
};

// Говоря о первом задании - я лекцию VK не смотрел и не особо понимаю практический смысл задания. Может он есть и я просто недалекий, но я решил сделать не по тз11
void task1(Bin_tree tree) {
    int max = tree.get_max();
    int min = tree.get_min();

    std::cout << max * min;
}

void task2(Bin_tree tree) {
    int min = tree.get_min();

    std::cout << min;
}

void task3(Bin_tree tree) {
    int max = tree.get_max();

    std::cout << max;

}

int main()
{
    Bin_tree tree;
    int n, m; std::cin >> n;
    for (int i = 0; i < n; ++i) {
        std::cin >> m;
        tree.insert(m);
    }

    task1(tree);
   // task2(tree);
   // task3(tree);
}

