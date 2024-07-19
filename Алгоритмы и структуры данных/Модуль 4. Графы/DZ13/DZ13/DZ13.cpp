// DZ13.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

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

    void testing(int vertex, int component) {
        dfs(vertex, component);
        for (int elem : visited) { std::cout << elem << " "; }
        std::cout << std::endl;
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

    std::vector<int> get_component_of_vertex(int vertex) {
        visited.assign(visited.size(), 0);
        dfs(vertex);
        return visited;
    }

private:
    std::vector<std::list<int>> gr;
    std::vector<int> visited;

    void dfs(int vertex) { // классический поиск в глубину
        visited[vertex] = 1;

        for (auto neighbour : gr[vertex]) {
            if (!visited[neighbour]) {
                dfs(neighbour, 1);
            }
        }
    }

    void dfs(int vertex, int component) { // выделение компонент связности
        visited[vertex] = component;

        for (auto neighbour : gr[vertex]) {
            if (!visited[neighbour]) {
                dfs(neighbour, component);
            }
        }
    }

    //void cycle_search(int vertex) { // поиск и восстановление цикла в графе
    //    visited[vertex] = 0;

    //    for (auto neighbour : gr[vertex]) {
    //        if (!visited[neighbour]) {
    //            dfs(neighbour, 1);
    //        }
    //    }
    //}
};


int main()
{
    int n, v1, v2; std::cin >> n;
    neograph graph; // с запасом
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

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"
