// 1) У вас есть граф, где каждая вершина представляет человека, а ребро между двумя вершинами означает, что эти два человека друзья. 
//    Найдите, существуют ли три человека, которые все взаимно дружат друг с другом (то есть между каждой парой из трех человек существует ребро).
// 2) Определите количество различных социальных кругов в графе. Социальный круг – это подграф, где все вершины связаны между собой напрямую или через других членов круга.
// Решено на основе шаблона из директории "Шаблоны структур данных и алгоритмов"

#include <iostream>
#include <vector>
#include <list>


class neograph {
public:
    neograph(int vertex = 1) {
        gr.resize(vertex);
        visited.resize(gr.size());
        visited.assign(visited.size(), 0);
    }

    void add_edge(int vertex1, int vertex2) {
        vertex1--; vertex2--;
        gr[vertex1].push_back(vertex2);
        gr[vertex2].push_back(vertex1);
    }

    void add_vertex() {
        gr.resize(gr.size() + 1);
        visited.resize(visited.size() + 1);
    }

    int size() {
        return gr.size();
    }

    std::vector<int> get_components() {
        visited.assign(visited.size(), 0);
        int comp = 1;
        for (int i = 0; i < visited.size(); ++i) {
            if (!visited[i]) {
                dfs(i, comp); 
                comp++;
            }
        }
        return visited;
    }

    bool get_cycle() {
        bool flag = false;
        for (int vertex = 1; vertex < gr.size(); ++vertex) {
            visited.assign(visited.size(), 0);
            memory.clear();
            flag = dfs_cycle_search(vertex - 1, vertex - 1, 1);
            if (flag) { return flag; }
        }
        return flag;
    }

private:
    std::vector<std::list<int>> gr;
    std::vector<int> visited;
    std::vector<int> memory;

    void dfs(int vertex, int component = 1) { // выделение компонент связности, при пустом параметре component классический поиск в глубину
        visited[vertex] = component;

        for (auto neighbour : gr[vertex]) {
            if (!visited[neighbour]) {
                dfs(neighbour, component);
            }
        }
    }

    bool dfs_cycle_search(int start_vertex, int vertex, int count) { // поиск и восстановление цикла в графе
        bool flag = false;
        visited[vertex] = 1; // 1 - "вершина была посещена в текущем рекурсивном обходе"
                             // 2 - "вершина была посещена ранее"
        if (count == 4) { return false; }

        for (auto neighbour : gr[vertex]) {
            if (!visited[neighbour]) {
                flag = dfs_cycle_search(start_vertex, neighbour, count + 1);
                if (flag) { return true; } // если мы нашли цикл, то завершаем функцию
            }
            else if ((visited[neighbour] == 1) && (neighbour == start_vertex) && (count == 3)) {
                return true;
            }
        }

        visited[vertex] = 2;

        return flag;
    }
};

void task1() {
    int n, v1, v2; std::cin >> n;
    neograph graph;
    for (int i = 0; i < n; ++i) {
        std::cin >> v1; std::cin >> v2;

        while ((graph.size() < v1) || (graph.size() < v2)) {
            graph.add_vertex();
        }

        graph.add_edge(v1, v2);
    }

    if (graph.get_cycle()) {
        std::cout << "YES";
    }
    else {
        std::cout << "NO";
    }
}

void task2() {
    int n, v1, v2; std::cin >> n;
    neograph graph;
    for (int i = 0; i < n; ++i) {
        std::cin >> v1; std::cin >> v2;

        while ((graph.size() < v1) || (graph.size() < v2)) {
            graph.add_vertex();
        }

        graph.add_edge(v1, v2);
    }

    std::vector<int> result = graph.get_components();

    int maxim = -1;
    for (int elem : result) {
        if (maxim < elem) { maxim = elem; }
    }

    std::cout << maxim;
}

int main()
{

}