
#include <iostream>
#include <vector>

template<typename type>
class Bin_tree {
protected:
    struct node { // налево идут все вершины <= а направо - все вершины >
        std::shared_ptr<node> parent = nullptr;
        std::shared_ptr<node> left = nullptr;
        std::shared_ptr<node> right = nullptr;
        int key;
        type info; // полезная нагрузка

        node(int input_key, type input_info) { key = input_key; info = input_info; }
        node() { }
    };

    std::shared_ptr<node> rootnode = nullptr;
    std::vector<std::pair<int, type>> result;

    void get_nodes(std::shared_ptr<node> n) {
        if (n == nullptr)
            return;
        get_nodes(n->left);
        result.push_back({ n->key, n->info });
        get_nodes(n->right);
    }

    std::vector<std::shared_ptr<node>> search_nodes(int key) {
        std::shared_ptr<node> pointer = rootnode;
        std::vector<std::shared_ptr<node>> result;
        while (pointer != nullptr) {
            if (key == pointer->key) {
                result.push_back(pointer);
                pointer = pointer->left;
            }
            else if (key < pointer->key)
                pointer = pointer->left;
            else if (key > pointer->key)
                pointer = pointer->right;
        }

        return result;
    }

    void delete_list(std::shared_ptr<node> n) {
        if (n->key <= n->parent->key)
            n->parent->left = nullptr; // вроде бы этого достаточно для удаления, потому что умный указатель должен автоматически очистить память, на которую ничего не указывает...
        else
            n->parent->right = nullptr;
    }

    void delete_node_with_2childs(std::shared_ptr<node> n) {
        std::shared_ptr<node> swap_node; node memory;
        swap_node = n->right;
        while (swap_node->left != nullptr) // находим самый левый узел правого поддерева
            swap_node = swap_node->left;

        // меняем местами самый левый узел правого поддерева и удаляемый узел
        memory.key = swap_node->key;
        memory.info = swap_node->info;
        swap_node->key = n->key;
        swap_node->info = n->info;
        n->key = memory.key;
        n->info = memory.info;

        if (swap_node->parent == n)
            swap_node->parent->right = nullptr;
        else
            swap_node->parent->left = nullptr;
    }

    void delete_node_with_1child(std::shared_ptr<node> n) {
        if (n->key <= n->parent->key)
            n->parent->left = (n->left == nullptr ? n->right : n->left);
        else
            n->parent->right = (n->left == nullptr ? n->right : n->left);
        n->left == nullptr ? n->right->parent = n->parent : n->left->parent = n->parent;
    }
public:
    void insert(int key, type info) { // ' ' - мусорное значение, которое подойдет как для числовых, так и для строковых типов данных
                                            // необходимо для удобного построения дерева без полезной нагрузки
        std::shared_ptr<node> pointer = rootnode;

        if (rootnode == nullptr) {
            rootnode = std::make_shared<node>();
            rootnode->key = key;
            rootnode->info = info;
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
                        pointer->left->info = info;
                        pointer->left->parent = pointer;
                        flag = false;
                    }
                else if (key > pointer->key)
                    if (pointer->right != nullptr)
                        pointer = pointer->right;
                    else {
                        pointer->right = std::make_shared<node>();
                        pointer->right->key = key;
                        pointer->right->info = info;
                        pointer->right->parent = pointer;
                        flag = false;
                    }
            }
        }
    }

    std::vector<std::pair<int, type>> contains(int key) {
        std::vector<std::pair<int, type>> result;
        std::vector<std::shared_ptr<node>> nodes;

        nodes = search_nodes(key);
        for (auto n : nodes)
            result.push_back({ n->key, n->info });

        return result;
    }

    void erase(int key) {
        std::vector<std::shared_ptr<node>> nodes = search_nodes(key);

        for (auto n : nodes) {
            if ((n->left == nullptr) && (n->right == nullptr))  // удаление листа - у веришны нет "потомков"
                delete_list(n);
            else if ((n->left != nullptr) && (n->right != nullptr)) // самый хреновый случай - узел с двумя потомками
                delete_node_with_2childs(n);
            else // если у узла только один потомок
                delete_node_with_1child(n);
        }
    }

    std::vector<std::pair<int, type>> get_nodes() {
        result.clear();
        get_nodes(rootnode);
        return result;
    }

    std::pair<int, type> get_max() {
        std::shared_ptr<node> pointer = rootnode;
        while (pointer->right != nullptr)
            pointer = pointer->right;

        return { pointer->key, pointer->info };
    }

    std::pair<int, type> get_min() {
        std::shared_ptr<node> pointer = rootnode;
        while (pointer->left != nullptr)
            pointer = pointer->left;

        return { pointer->key, pointer->info };
    }
};

int main()
{
    Bin_tree <int> tree;

    tree.insert(50, 1);
    tree.insert(60, 1);
    tree.insert(70, 1);
    tree.insert(30, 1);
    tree.insert(35, 1);
    tree.insert(15, 1);
    //tree.insert(33);
    //tree.insert(40);
    //tree.insert(37);
    //tree.insert(45);

    //std::vector<std::pair<int, int>> result = tree.get_nodes();
    //for (auto elem : result) {
    //    std::cout << elem.first << " " << elem.second << "\n";
    //}

    //std::cout << "TEST" << "\n";
    //tree.erase(30);

    //result = tree.get_nodes();
    //for (auto elem : result) {
    //    std::cout << elem.first << " " << elem.second << "\n";
    //}
    
    std::pair<int, int> result = tree.get_max();

    std::cout << result.first << " " << result.second;

    int n = 5;
}
