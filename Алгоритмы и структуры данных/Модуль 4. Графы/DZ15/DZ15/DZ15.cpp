// Решения на основе класса графа из папки Шаблонов этого же репозитория
// 1) Дан ориентированный взвешенный граф, представленный в виде списка рёбер. Напишите функцию на C++, которая находит кратчайшие пути от заданной начальной вершины 
//    до всех остальных вершин с использованием алгоритма Беллмана-Форда. Функция также должна определять наличие отрицательных циклов в графе.
// 2) Дан неориентированный граф, представленный в виде списка смежности. Напишите функцию на C++, которая находит все мосты в графе. 
//    Мостом в графе называется ребро, удаление которого увеличивает количество компонент связности.
#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <queue>
#include <set>
#include <Windows.h>  

class Neograph {
public:
    Neograph(int vertex = 1) {
        gr.resize(vertex);
        visited.resize(gr.size());
        visited.assign(visited.size(), 0);
    }

    void add_edge(int vertex1, int vertex2, int weight = 1) {
        while ((gr.size() < vertex1) || (gr.size() < vertex2)) { // граф автоматически дозаполняется, если была передана вершина с номером большим, чем размер графа
            add_vertex();
        }
        vertex1--; vertex2--;
        gr[vertex1].push_back(std::make_pair(vertex2, weight));
        gr[vertex2].push_back(std::make_pair(vertex1, weight));
    }

    void add_vertex() {
        gr.resize(gr.size() + 1);
        visited.resize(visited.size() + 1);
    }

    int size() {
        return gr.size();
    }

    std::vector<std::pair<int, int>> get_bridges() {
        // не забудт инициализировать memory и memory_add -1
        // visited 0
        memory.assign(gr.size(), 0);
        memory_add.assign(gr.size(), 0);
        visited.assign(gr.size(), 0);
        result.clear();

        dfs_bridge_search(0, -1);

        return result;
    }

protected:
    const int INF = 1e9;            // константа условной бесконечности
    std::vector<std::list<
        std::pair<int, int>>        // <номер вершины, вес ведущего в него ребра>
        > gr;          // gr содержит информацию о связях в вершинах
    // inverted_gr - вспомогательный инвертированный граф, используемый в некоторых алгоритмах
    std::vector<int> visited;       // используются в функциях и процедурах для хранения промежуточных и итоговых значений
    std::vector<int> memory;
    std::vector<int> memory_add;    // memory_add и result инициализированы только для алгоритма поиска мостов
    std::vector<std::pair<int, int>> result;

    // Прямое ребро - ребро, которое ведет в вершину, которая еще не была посещена dfs
    // Обратное ребро - ребро, которое ведет в вершину, которая уже была посещена dfs
    void dfs_bridge_search(int cur_v, int pred_v) {
        visited[cur_v] = 1; // Помечаем вершину как посещенную
        memory_add[cur_v] = (pred_v == -1 ? 0 : memory[pred_v] + 1); // в memory_add будем хранить "лучшие" глубины, а в memory глубины как они встречались в обходе dfs
        memory[cur_v] = (pred_v == -1 ? 0 : memory[pred_v] + 1);     // в ? проверка на -1, чтобы гарантировать корректное обращение к элементу массива

        for (auto neighbour : gr[cur_v]) { // Обходим соседей вершины
            if(neighbour.first != pred_v)
                if (!visited[neighbour.first]) { // если ребро - прямое, то
                    dfs_bridge_search(neighbour.first, cur_v); // запускаем обход от этого ребра
                    if (memory_add[neighbour.first] < memory_add[cur_v]) { memory_add[cur_v] = memory_add[neighbour.first]; } // После обхода проверяем - если лучшая глубина соседа меньше лучшей глубины текущей вершины, то
                    // перезаписываем лучшую глубину текущей вершины. Если описанное условие выполняется, то это значит, что
                    // найденная раннее глубина вершины не является лучшей, вот мы ее и перезаписываем
                    if (memory[cur_v] < memory_add[neighbour.first]) { result.push_back({ cur_v, neighbour.first }); }        // Если условие выполняется, то ребро является мостом - записываем его в результат
                }
                else { // если ребро - обратное и глубина соседа "лучше" глубины текущей веришны, то перезаписываем лучшую глубину текущей вершины
                    if (memory[neighbour.first] < memory_add[cur_v]) { memory_add[cur_v] = memory[neighbour.first]; }
                }
        }
    }
};


class Ograph : public Neograph {
public:
    Ograph(int vertex = 1) : Neograph(vertex) { }

    void add_edge(int vertex1, int vertex2, int weight = 1) {
        while ((gr.size() < vertex1) || (gr.size() < vertex2)) { // граф автоматически дозаполняется, если была передана вершина с номером большим, чем размер графа
            add_vertex();
        }
        vertex1--; vertex2--;
        gr[vertex1].push_back(std::make_pair(vertex2, weight));
    }

    bool has_negative_cycle() {
        get_BellmanFord();

        bool flag = false;
        for (int vertex = 0; vertex < gr.size(); ++vertex)
            for (std::pair<int, int> edge : gr[vertex])
                if (visited[vertex] + edge.second < visited[edge.first]) {
                    visited[edge.first] = visited[vertex] + edge.second;
                    flag = true;
                }

        return flag;
    }

    std::vector<int> get_BellmanFord(int start_vertex = 1) { // Алгоритм Беллмана-Форда поиска кратчайшего пути (работает на взвешенных графах с отрицательными ребрами! но не должно быть "отрицательных циклов")
        // Вообще для алгоритма Беллмана-Форда удобнее хранить граф в виде массива структур с 3 полями - вершина, вершина, вес ребра между ними
        start_vertex--; bool flag = true;
        visited.assign(gr.size(), INF); // массив с текущими расстояниями до вершин
        visited[start_vertex] = 0;

        for (int i = 0; i < gr.size(); ++i) // почему именно такой цикл? потому что за V итераций мы гарантированно найдем все растояния (V - количество вершин)
            for (int vertex = 0; vertex < gr.size(); ++vertex)
                for (std::pair<int, int> edge : gr[vertex])
                    if (visited[vertex] + edge.second < visited[edge.first])
                        visited[edge.first] = visited[vertex] + edge.second;

        return visited;
    }
};

void task1() {
    Ograph graph;

    int m, v1, v2, w;

    std::cin >> m;

    for (int i = 0; i < m; ++i) {
        std::cin >> v1 >> v2 >> w;
        graph.add_edge(v1, v2, w); // + 1 из-за особенности реализации добавления ребра в классе 
    }

    if (!graph.has_negative_cycle()) {
        std::vector<int> result = graph.get_BellmanFord();
        std::string ans;
        for (int vertex = 0; vertex < graph.size(); ++vertex) {
            if (vertex != 0) { std::cout << "; "; }
            std::cout << "Вершина: " << vertex << ", Расстояние: " << result[vertex];
        }
    }
    else { std::cout << "Обнаружен негативный цикл"; }
}

void task2() {
    Neograph graph;

    int n, m, v1, v2;

    std::cin >> n >> m;

    for (int i = 0; i < m; ++i) {
        std::cin >> v1 >> v2;
        graph.add_edge(v1 + 1, v2 + 1); // + 1 из-за особенности реализации добавления ребра в классе 
    }

    std::vector<std::pair<int, int>> result = graph.get_bridges();

    for (auto elem : result) {
        std::cout << "Мост: " << elem.first << " - " << elem.second << "\n";
    }
}

int main()
{
}
