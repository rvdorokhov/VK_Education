// neograph.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <vector>
#include <list>
#include <stack>


class Neograph {
public:
    Neograph(int vertex = 1) {
        gr.resize(vertex);
        visited.resize(gr.size());
        visited.assign(visited.size(), 0);
    }

    void add_edge(int vertex1, int vertex2) {
        while ((gr.size() < vertex1) || (gr.size() < vertex2)) { // граф автмоатически дозаполняется, если была передана вершина с номером большим, чем размер графа
            add_vertex();
        }
        vertex1--; vertex2--;
        gr[vertex1].push_back(vertex2);
        gr[vertex2].push_back(vertex1);
    }

    void delete_edge(int vertex1, int vertex2) {
        vertex1--; vertex2--;
        gr[vertex1].remove(vertex2);
        gr[vertex2].remove(vertex1);
    }

    void add_vertex() {
        gr.resize(gr.size() + 1);
        visited.resize(visited.size() + 1);
    }

    void print_edges() {
        for (int vertex = 0; vertex < gr.size(); ++vertex) {
            std::cout << vertex + 1 << ": ";
            for (auto neighbour : gr[vertex]) {
                std::cout << neighbour + 1 << " ";
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

    std::vector<int> get_cycle(int vertex) {
        visited.assign(visited.size(), 0);
        cycle.clear();
        dfs_cycle_search(vertex - 1, vertex - 1, -1);
        for (int i = 0; i < cycle.size(); ++i) { cycle[i]++; }
        return cycle;
    }

    std::vector<int> get_cycle() {
        for (int vertex = 1; vertex < gr.size(); ++vertex) {
            visited.assign(visited.size(), 0);
            cycle.clear();
            dfs_cycle_search(vertex - 1, vertex - 1, -1);
            if (!cycle.empty()) { 
                for (int i = 0; i < cycle.size(); ++i) { cycle[i]++; }
                return cycle; 
            }
        }
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

protected:
    std::vector<std::list<int>> gr; // содержит информацию о связях в вершинах
    std::vector<int> visited;       // используются в функциях и процедурах для хранения промежуточных и итоговых значений
    std::vector<int> cycle;

    void dfs(int vertex, int component = 1) { // поиск в глубину (в таблице visited помечает как component все достижимые вершины, 
                                              // другими словами - находит компоненту связности текущей веришны
        visited[vertex] = component;

        for (auto neighbour : gr[vertex]) {
            if (!visited[neighbour]) {
                dfs(neighbour, component);
            }
        }
    }

    bool dfs_cycle_search(int start_vertex, int vertex, int parent = -1) { // поиск и восстановление цикла в графе
        bool flag = false;
        visited[vertex] = 1; // 1 - "вершина была посещена в текущем рекурсивном обходе"
                             // 2 - "вершина была посещена ранее"
        cycle.push_back(vertex);

        for (auto neighbour : gr[vertex]) {
            if (!visited[neighbour]) {
                flag = dfs_cycle_search(start_vertex, neighbour, vertex);
                if (flag) { return true; } // если мы нашли цикл, то завершаем функцию
            }
            else if ((visited[neighbour] == 1) && (neighbour != parent) && (neighbour == start_vertex)) {
                return true;
            }
        }

        cycle.pop_back();
        visited[vertex] = 2;

        return flag;
    }

    bool dfs_bipartite(int vertex, int colour) {
        visited[vertex] = colour; bool flag = true;

        for (auto neighbour : gr[vertex]) {
            if (!visited[neighbour]) {
                flag = dfs_bipartite(neighbour, 3 - colour); // если пометить цвета как 1 и 2, то 3 - 1 = 2 и 3 - 2 = 1, 
                                                             // что очень удобно при передачи в рекурсивный вызов функции
                if (!flag) { return false; }
            }
            else if (visited[neighbour] == colour) {
                return false;
            }
        }

        return flag;
    }
private:
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
};

class Ograph : public Neograph {
public:
    Ograph(int vertex = 1) : Neograph(vertex) { }

    void add_edge(int vertex1, int vertex2) {
        while ((gr.size() < vertex1) || (gr.size() < vertex2)) { // граф автмоатически дозаполняется, если была передана вершина с номером большим, чем размер графа
            add_vertex();
        }
        vertex1--; vertex2--;
        gr[vertex1].push_back(vertex2);
    }

    void delete_edge(int vertex1, int vertex2) {
        vertex1--; vertex2--;
        gr[vertex1].remove(vertex2);
    }

    bool dfs_bipartite(int vertex, int colour) {
        // тут надо сначала построить вспомогательный неограф, затем вызвать от него функцию определения двудольности родительского класса
    }
};

int main()
{
    Neograph graph;

    //graph.add_edge(1, 2); graph.add_edge(1, 3); graph.add_edge(4, 5);
    //graph.add_edge(5, 1); graph.add_edge(4, 3); graph.add_edge(3, 2);

    //graph.add_edge(1, 2); graph.add_edge(2, 3); graph.add_edge(3, 4); graph.add_edge(4, 1);


    graph.print_edges();

    std::vector<int> result = graph.bipartite_parts();

    for (int elem : result) {
        std::cout << elem << " ";
    }
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"
