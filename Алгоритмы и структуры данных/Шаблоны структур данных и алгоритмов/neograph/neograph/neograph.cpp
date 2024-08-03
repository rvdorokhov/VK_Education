#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <queue>

class Neograph {
public:
    Neograph(int vertex = 1) {
        gr.resize(vertex);
        visited.resize(gr.size());
        visited.assign(visited.size(), 0);
    }

    void add_edge(int vertex1, int vertex2, int weight = 1) {
        while ((gr.size() < vertex1) || (gr.size() < vertex2)) { // граф автмоатически дозаполняется, если была передана вершина с номером большим, чем размер графа
            add_vertex();
        }
        vertex1--; vertex2--;
        gr[vertex1].push_back(std::make_pair(vertex2, weight));
        gr[vertex2].push_back(std::make_pair(vertex1, weight));
    }

    void delete_edge(int vertex1, int vertex2) {
        vertex1--; vertex2--;
        gr[vertex1].remove_if([=](std::pair<int, int> edge) { return edge.first == vertex2; });  //vertex2
        gr[vertex2].remove_if([=](std::pair<int, int> edge) { return edge.first == vertex1; });  //vertex1
    }

    void add_vertex() {
        gr.resize(gr.size() + 1);
        visited.resize(visited.size() + 1);
    }

    void print_edges() {
        for (int vertex = 0; vertex < gr.size(); ++vertex) {
            std::cout << vertex + 1 << ": ";
            for (auto neighbour : gr[vertex]) {
                std::cout << neighbour.first + 1 << " ";
            }
            std::cout << std::endl;
        }
    }

    int size() {
        return gr.size();
    }

    std::vector<int> get_components(int vertex) { // поиск компоненты связности конкретной вершины
        visited.assign(visited.size(), 0);
        dfs(vertex - 1, 1);
        return visited;
    }

    std::vector<int> get_components() { // поиск всех компонент связности
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

    std::vector<int> get_cycle(int vertex) {
        visited.assign(visited.size(), 0);
        memory.clear();
        dfs_cycle_search(vertex - 1, vertex - 1, -1);
        for (int i = 0; i < memory.size(); ++i) { memory[i]++; }
        return memory;
    }

    std::vector<int> get_cycle() {
        for (int vertex = 1; vertex < gr.size(); ++vertex) {
            visited.assign(visited.size(), 0);
            memory.clear();
            dfs_cycle_search(vertex - 1, vertex - 1, -1);
            if (!memory.empty()) { 
                for (int i = 0; i < memory.size(); ++i) { memory[i]++; }
                return memory; 
            }
        }
        return memory;
    }

    std::vector<int> bipartite_parts() {
        visited.assign(visited.size(), 0); 
        bool flag = true;
        for (int vertex = 0; (vertex < gr.size())&&(flag); ++vertex) {
            if (!visited[vertex]) {
                flag = dfs_bipartite(vertex, 1);
            }
        }

        if (!flag) {
            visited.assign(visited.size(), 0);
        }
        return visited;
    }

    std::vector<int> get_bfs(int vertex = 1) {
        visited.assign(visited.size(), INF);
        bfs(vertex - 1);
        return visited;
    }

protected:
    const int INF = 10e9;           // константа условной бесконечности
    std::vector<std::list<
        std::pair<int, int>>        // <номер вершины, вес ведущего в него ребра>
        > gr;                       // содержит информацию о связях в вершинах
    std::vector<int> visited;       // используются в функциях и процедурах для хранения промежуточных и итоговых значений
    std::vector<int> memory;

    void dfs(int vertex, int component = 1) { // поиск в глубину (в таблице visited помечает как component все достижимые вершины, 
                                              // другими словами - находит компоненту связности текущей веришны)
        visited[vertex] = component;

        for (auto neighbour : gr[vertex]) {
            if (!visited[neighbour.first]) {
                dfs(neighbour.first, component);
            }
        }

    }

    bool dfs_cycle_search(int start_vertex, int vertex, int parent = -1) { // поиск и восстановление цикла в графе
        bool flag = false;
        visited[vertex] = 1; // 1 - "вершина была посещена в текущем рекурсивном обходе"
                             // 2 - "вершина была посещена ранее"
        memory.push_back(vertex);

        for (auto neighbour : gr[vertex]) {
            if (!visited[neighbour.first]) {
                flag = dfs_cycle_search(start_vertex, neighbour.first, vertex);
                if (flag) { return true; } // если мы нашли цикл, то завершаем функцию
            }
            else if ((visited[neighbour.first] == 1) && (neighbour.first != parent) && (neighbour.first == start_vertex)) {
                return true;
            }
        }

        memory.pop_back();
        visited[vertex] = 2;

        return flag;
    }

    void bfs(int start_vertex) {
        std::queue<int> q; int cur_vertex;

        q.push(start_vertex);
        visited[start_vertex] = 0;

        while (!q.empty()) {
            cur_vertex = q.front();
            q.pop();
            for (std::pair<int, int> vertex : gr[cur_vertex]) {
                if (visited[vertex.first] > visited[cur_vertex] + vertex.second) {
                    visited[vertex.first] = visited[cur_vertex] + vertex.second;
                    q.push(vertex.first);
                }
            }
        }
    }
private:
    bool dfs_bipartite(int vertex, int colour) {
        visited[vertex] = colour; bool flag = true;

        for (auto neighbour : gr[vertex]) {
            if (!visited[neighbour.first]) {
                flag = dfs_bipartite(neighbour.first, 3 - colour); // если пометить цвета как 1 и 2, то 3 - 1 = 2 и 3 - 2 = 1, 
                // что очень удобно при передачи в рекурсивный вызов функции
                if (!flag) { return false; }
            }
            else if (visited[neighbour.first] == colour) {
                return false;
            }
        }

        return flag;
    }
};

class Ograph : public Neograph {
public:
    Ograph(int vertex = 1) : Neograph(vertex) { }

    void add_edge(int vertex1, int vertex2, int weight = 1) {
        while ((gr.size() < vertex1) || (gr.size() < vertex2)) { // граф автмоатически дозаполняется, если была передана вершина с номером большим, чем размер графа
            add_vertex();
        }
        vertex1--; vertex2--;
        gr[vertex1].push_back(std::make_pair(vertex2, weight));
    }

    void delete_edge(int vertex1, int vertex2) {
        vertex1--; vertex2--;
        gr[vertex1].remove_if([=](std::pair<int, int> edge) { return edge.first == vertex2; });
    }

    std::vector<int> bipartite_parts() {
        // тут надо сначала построить вспомогательный неограф, затем вызвать от него функцию определения двудольности родительского класса
        Neograph neogr(gr.size()); // вспомогательный неориентированный граф
        for (int i = 0; i < gr.size(); ++i) { // заполняем вспомогательный неориентированный граф
            for (std::pair<int, int> edge : gr[i]) {
                neogr.add_edge(i + 1, edge.first + 1);
            }
        }

        return neogr.bipartite_parts();
    }

    std::vector<int> get_topologic() { // топлогическая сортировка определена только на гарфах без циклов, однако описанный алгоритм это не учитывает
        for (int i = 0; i < visited.size(); ++i) { // запускаем топлогическую сортировку от каждой непосещенной вершины (на случай если граф несвязный)
            if (!visited[i]) {
                dfs_topologic(i);
            }
        }

        for (int& elem : memory) {
            elem++;
        }

        std::reverse(memory.begin(), memory.end());
        return memory;
    }

    std::vector<int> get_components() { // Алгоритм Косараджу-Шарира
        Ograph inverted_gr(gr.size()); // вспомогательный инверитрованный граф
        for (int i = 0; i < gr.size(); ++i) { // заполняем вспомогательный неориентированный граф
            for (std::pair<int, int> edge : gr[i]) {
                inverted_gr.add_edge(i + 1, edge.first + 1);
            }
        }
        memory = inverted_gr.get_topologic();   // запускаем "топлогическую" сортировку от каждой непосещенной вершины (на случай если граф несвязный)
        for (int& elem : memory) { --elem; }    // в результате работы цикла имеет в memory "топологически" отсортированный массив
                                                // топлогический условно, потому что топологическая сортировка определена только на графах без циклов
        int colour = 1;
        for (int i = 0; i < gr.size(); ++i) {
            if (!visited[memory[i]]) {
                dfs(memory[i], colour);
                ++colour;
            }
        }

        return visited;
    }
private:
    void dfs_topologic(int vertex) { // поиск в глубину (в таблице visited помечает как component все достижимые вершины, 
        // другими словами - находит компоненту связности текущей веришны
        visited[vertex] = 1;

        for (auto neighbour : gr[vertex]) {
            if (!visited[neighbour.first]) {
                dfs_topologic(neighbour.first);
            }
        }

        memory.push_back(vertex);
    }
};

int main()
{
    Neograph graph;

    graph.add_edge(1, 2); graph.add_edge(1, 4);
    graph.add_edge(2, 9); 
    graph.add_edge(9, 3); graph.add_edge(9, 6);
    graph.add_edge(6, 7); graph.add_edge(7, 3);
    graph.add_edge(3, 4); graph.add_edge(3, 5); graph.add_edge(3, 10);
    graph.add_edge(4, 8); graph.add_edge(4, 10);
    graph.add_edge(8, 10);

    std::vector<int> result = graph.get_bfs();


    for (int elem : result) {
        std::cout << elem << " ";
    }
}
