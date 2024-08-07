// Решения на основе класса графа из папки Шаблонов этого же репозитория
// 1) Найдите самый быстрый путь от одного перекрестка до другого.
// 2) Найдите путь с минимальным количеством пересадок между станциями.

#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <queue>
#include <set>

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

    // ТОЛЬКО ДЛЯ НЕВЗВЕШЕННОГО ГРАФА
    std::vector<int> get_bfs(int vertex = 1) { // классический поиск в ширину - кратчайших расстояний до каждой вершины из заданной на невзвешенном графе
        visited.assign(visited.size(), INF);
        memory.assign(visited.size(), 0);
        bfs(vertex - 1);
        return visited;
    }

    std::vector<int> get_Dijkstra(int start_vertex = 1) { // Алгоритм Дейкстры поиска кратчайшего пути (работает на взвешенных графах! но только с неотрицательными весами)
        // наивная реализация - с помощью массива. Есть реализация с помощью std::set для хранения текущих расстояний, 
        // но мне лень писать еще одну Дейкстру/
        start_vertex--;
        int minim, vertex_minim;
        std::vector<bool> cur_mins(gr.size());    // массив флагов. 1 значит, что расстояние до вершины минимально
        visited.assign(gr.size(), INF); // массив с текущими расстояниями до вершин
        visited[start_vertex] = 0;


        for (int i = 0; i < gr.size(); ++i) { // т.к. мы гарантированно найдем все расстояния за V повторов (V - количество вершин)
            minim = INF;
            for (int vertex = 0; vertex < visited.size(); ++vertex) // определение текущего минимального расстояния
                if ((visited[vertex] < minim) && (!cur_mins[vertex])) {
                    vertex_minim = vertex;
                    minim = visited[vertex];
                }
            cur_mins[vertex_minim] = 1;

            for (std::pair<int, int> neighbour : gr[vertex_minim]) {
                if (minim + neighbour.second < visited[neighbour.first])
                    visited[neighbour.first] = minim + neighbour.second;
            }
        }

        return visited;
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

    // ТОЛЬКО ДЛЯ НЕВЗВЕШЕННОГО ГРАФА 
    void bfs(int start_vertex) { // поиск в ширину позволяющий восстанавливать кратчайший путь
        std::queue<int> q; int cur_vertex;

        q.push(start_vertex);
        visited[start_vertex] = 0;

        while (!q.empty()) {
            cur_vertex = q.front();
            q.pop();
            for (std::pair<int, int> vertex : gr[cur_vertex]) {
                if (visited[vertex.first] > visited[cur_vertex] + 1) {
                    visited[vertex.first] = visited[cur_vertex] + 1;
                    memory[vertex.first] = cur_vertex;
                    q.push(vertex.first);
                }
            }
        }
    }
};

void task1() {
    Neograph graph;

    int m, v1, v2, w;

    std::cin >> m;

    for (int i = 0; i < m; ++i) {
        std::cin >> v1 >> v2 >> w;
        graph.add_edge(v1, v2, w); // + 1 из-за особенности реализации добавления ребра в классе 
    }
    std::cin >> m >> w;

    std::vector<int> result = graph.get_Dijkstra(m);

    if (result[w - 1] != 1e9) { // 1e9 - константа бесконечности в графе
        std::cout << result[w - 1];
    } else { std::cout << "No path found"; }
}

void task2() {
    Neograph graph;

    int m, v1, v2;

    std::cin >> m;

    for (int i = 0; i < m; ++i) {
        std::cin >> v1 >> v2;
        graph.add_edge(v1, v2); // + 1 из-за особенности реализации добавления ребра в классе 
    }
    std::cin >> v1 >> v2;

    std::vector<int> result = graph.get_bfs(v1); // Дейкстра из первой задачи тоже пойдет

    if (result[v2 - 1] != 1e9) { // 1e9 - константа бесконечности в графе
        std::cout << result[v2 - 1] - 1;
    }
    else { std::cout << "No path found"; }
}

int main()
{
    task2();
}