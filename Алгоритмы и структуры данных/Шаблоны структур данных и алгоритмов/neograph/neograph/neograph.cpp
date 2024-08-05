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

    std::vector<int> get_cycle(int vertex) { // поиск цикла от конкретной веришны
        visited.assign(visited.size(), 0);
        memory.clear();
        dfs_cycle_search(vertex - 1, vertex - 1, -1);
        for (int i = 0; i < memory.size(); ++i) { memory[i]++; }
        return memory;
    }

    std::vector<int> get_cycle() { // поиск цикла - любого (по сути проверка на наличие цикла в кграфе)
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

    std::vector<int> bipartite_parts() { // проверка на двудольность
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

    // ТОЛЬКО ДЛЯ НЕВЗВЕШЕННОГО ГРАФА
    std::vector<int> get_bfs(int vertex = 1) { // классический поиск в ширину - кратчайших расстояний до каждой вершины из заданной на невзвешенном графе
        visited.assign(visited.size(), INF);
        memory.assign(visited.size(), 0);
        bfs(vertex - 1);
        return visited;
    }

    // ТОЛЬКО ДЛЯ НЕВЗВЕШЕННОГО ГРАФА
    std::vector<int> get_shortest_path(int start_vertex, int end_vertex) { // восстановление кратчайшего пути от вершины до вершины
        --start_vertex; --end_vertex;
        visited.assign(visited.size(), INF);
        memory.assign(visited.size(), -1);      // в memory для каждой вершины будем хранить номер предыдущей вершины кратчайшего пути
        bfs(start_vertex);
        visited.clear(); // нас интересует кратчайший путь и массив memory
                         // для экономии памяти я не буду инициализировать дополнительный массив-результат, в качестве результата будет возвращен visited

        int vertex = end_vertex;
        while (vertex != start_vertex) {  // восстановление пути
            visited.push_back(vertex + 1);
            vertex = memory[vertex];
        }
        visited.push_back(vertex + 1); // добавление первой вершины

        std::reverse(visited.begin(), visited.end());
        return visited;
    }

    // ТОЛЬКО ДЛЯ НЕВЗВЕШЕННОГО ГРАФА
    std::vector<int> get_shortest_vertexes(int start_vertex, int end_vertex) { // поиск ВСЕХ вершин, которые могут содержаться в кратчайшем пути
                                                                               // нужны явный "исток" и явный "сток" (насколько это определимо в неографе)
        std::vector<int> bfs_result_start(gr.size()); // тут будут храниться кратчайшие пути от стартовой вершины 
        std::vector<int> bfs_result_end(gr.size()); // тут будут храниться кратчайшие пути от конечной вершины

        bfs_result_start = get_bfs(start_vertex);
        bfs_result_end = get_bfs(end_vertex);
        
        shortest_vertexes(end_vertex - 1, bfs_result_start, bfs_result_end);

        for (int& elem : memory) { ++elem; }

        return memory;
    }

    // ТОЛЬКО ДЛЯ НЕВЗВЕШЕННОГО ГРАФА
    std::vector<std::pair<int, int>> get_shortest_edges(int start_vertex, int end_vertex) { // поиск ВСЕХ ребер, которые могут содержаться в кратчайшем пути
                                                                                            // нужны явный "исток" и явный "сток" (насколько это определимо в неографе)
        std::vector<std::pair<int, int>> result;
        std::vector<int> bfs_result_start(gr.size()); // тут будут храниться кратчайшие пути от стартовой вершины 
        std::vector<int> bfs_result_end(gr.size()); // тут будут храниться кратчайшие пути от конечной вершины

        bfs_result_start = get_bfs(start_vertex);
        bfs_result_end = get_bfs(end_vertex);

        result = shortest_edges(end_vertex - 1, bfs_result_start, bfs_result_end);

        for (std::pair<int, int> &elem : result) { ++elem.first; ++elem.second; }

        return result;
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

            for (std::pair<int, int> neighbour: gr[vertex_minim]) {
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

    void shortest_vertexes(int end_vertex, std::vector<int> bfs_result_start, std::vector<int> bfs_result_end) {
        memory.clear(); // тут будет результат
        for (int i = 0; i < gr.size(); ++i) {
            if (bfs_result_start[i] + bfs_result_end[i] == bfs_result_start[end_vertex]) { // если сумма расстояний до каждой вершины равна кратчайшему пути
                // то вершина может быть в кратчайшем пути
                memory.push_back(i);
            }
        }
    }

    std::vector<std::pair<int, int>> shortest_edges(int end_vertex, std::vector<int> bfs_result_start, std::vector<int> bfs_result_end) {
        std::vector<std::pair<int, int>> result;
        for (int vertex = 0; vertex < gr.size(); ++vertex) {
            for (std::pair<int, int> neighbour : gr[vertex]) {
                if ((bfs_result_start[vertex] + bfs_result_end[neighbour.first] + 1 == bfs_result_start[end_vertex])) {
                    result.push_back(std::make_pair(vertex, neighbour.first));
                }
            }
        }

        return result;
    }

private:
    bool dfs_bipartite(int vertex, int colour) { // модификация дфс для проверки на двудольность
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
        while ((gr.size() < vertex1) || (gr.size() < vertex2)) { // граф автоматически дозаполняется, если была передана вершина с номером большим, чем размер графа
            add_vertex();
        }
        vertex1--; vertex2--;
        gr[vertex1].push_back(std::make_pair(vertex2, weight));
    }

    void delete_edge(int vertex1, int vertex2) {
        vertex1--; vertex2--;
        gr[vertex1].remove_if([=](std::pair<int, int> edge) { return edge.first == vertex2; });
    }

    std::vector<int> bipartite_parts() { // проверка на двудольность
        // тут надо сначала построить вспомогательный неограф, затем вызвать от него функцию определения двудольности родительского класса
        Neograph neogr(gr.size()); // вспомогательный неориентированный граф
        for (int i = 0; i < gr.size(); ++i) { // заполняем вспомогательный неориентированный граф
            for (std::pair<int, int> edge : gr[i]) {
                neogr.add_edge(i + 1, edge.first + 1);
            }
        }

        return neogr.bipartite_parts();
    }

    std::vector<int> get_topologic() { // топлогическая сортировка определена только на графах без циклов, однако описанный алгоритм это не учитывает
        visited.assign(visited.size(), 0);
        for (int i = 0; i < visited.size(); ++i) { // запускаем топологическую сортировку от каждой непосещенной вершины (на случай если граф несвязный)
            if (!visited[i]) {
                dfs_topologic(i);
            }
        }

        for (int& elem : memory) { elem++; }

        std::reverse(memory.begin(), memory.end());
        return memory;
    }

    std::vector<int> get_components() { // Алгоритм Косараджу-Шарира поиска компонент сильной связности ографа
        visited.assign(visited.size(), 0);
        memory.clear();
        Ograph inverted_gr = get_inverted_gr(); // вспомогательный инверитрованный граф

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

    // ТОЛЬКО ДЛЯ НЕВЗВЕШЕННОГО ГРАФА
    std::vector<int> get_shortest_vertexes(int start_vertex, int end_vertex) { // нужны явный "исток" и явный "сток" (насколько это определимо в неографе)
        std::vector<int> bfs_result_start(gr.size()); // тут будут храниться кратчайшие пути от стартовой вершины 
        std::vector<int> bfs_result_end(gr.size()); // тут будут храниться кратчайшие пути от конечной вершины
        Ograph inverted_gr = get_inverted_gr(); // вспомогательный инверитрованный граф

        bfs_result_start = get_bfs(start_vertex);
        bfs_result_end = inverted_gr.get_bfs(end_vertex);

        shortest_vertexes(end_vertex - 1, bfs_result_start, bfs_result_end);

        for (int& elem : memory) { ++elem; }

        return memory;
    }

    // ТОЛЬКО ДЛЯ НЕВЗВЕШЕННОГО ГРАФА
    std::vector<std::pair<int, int>> get_shortest_edges(int start_vertex, int end_vertex) { // нужны явный "исток" и явный "сток" (насколько это определимо в неографе)
        std::vector<std::pair<int, int>> result;
        std::vector<int> bfs_result_start(gr.size()); // тут будут храниться кратчайшие пути от стартовой вершины 
        std::vector<int> bfs_result_end(gr.size()); // тут будут храниться кратчайшие пути от конечной вершины
        Ograph inverted_gr = get_inverted_gr(); // вспомогательный инверитрованный граф

        bfs_result_start = get_bfs(start_vertex);
        bfs_result_end = inverted_gr.get_bfs(end_vertex);

        result = shortest_edges(end_vertex - 1, bfs_result_start, bfs_result_end);

        for (std::pair<int, int>& elem : result) { ++elem.first; ++elem.second; }

        return result;
    }

    std::vector<int> get_BellmanFord(int start_vertex = 1) { // Алгоритм Беллмана-Форда поиска кратчайшего пути (работает на взвешенных графах с отрицательными ребрами! но не должно быть "отрицательных циклов")
                                                             // Я не реализовал этот алгоритм для неографа потому что из-за особенности хранения графа любой 
                                                             // неограф с хотя бы одним отрицательным ребром имеет отрицательный цикл, в котором алгоритм застревает
                                                             // Вообще для алгоритма Беллмана-Форда удобнее хранить граф в виде массива структур с 3 полями - вершина, вершина, вес ребра между ними
        start_vertex--; bool flag = true;
        visited.assign(gr.size(), INF); // массив с текущими расстояниями до вершин
        visited[start_vertex] = 0;

        while (flag) {
            flag = false;
            for (int vertex = 0; vertex < gr.size(); ++vertex)
                for (std::pair<int, int> edge : gr[vertex])
                    if (visited[vertex] + edge.second < visited[edge.first]) {
                        visited[edge.first] = visited[vertex] + edge.second;
                        flag = true;
                    }
        }

        return visited;
    }

    std::vector<int> get_SPFA(int start_vertex = 1) { // Алгоритм Мура или SPFA
        start_vertex--; 
        int vertex;
        visited.assign(gr.size(), INF); // массив с текущими расстояниями до вершин
        std::queue<int> q;
        visited[start_vertex] = 0; q.push(start_vertex);

        while (!q.empty()) {
            vertex = q.front();
            q.pop();
            for (std::pair<int, int> edge : gr[vertex])
                if (visited[vertex] + edge.second < visited[edge.first]) {
                    visited[edge.first] = visited[vertex] + edge.second;
                    q.push(edge.first);
                }
        }

        return visited;
    }

    std::vector<int> smart_search(int start_vertex = 1) { // Умный поиск кратчайшего пути в ациклическом ориентирвоанном графе с произвольными весами
                                                          // https://www.youtube.com/watch?v=JakhzSl1JN0
        start_vertex--;
        memory.clear();                 // тут будет храниться результат топологической сортировки
        memory = get_topologic();
        for (int& vertex : memory) { --vertex; }

        visited.assign(gr.size(), INF); // массив с текущими расстояниями до вершин, переопределяем потому что его испортил get_topologic
        visited[start_vertex] = 0;
        for (int vertex : memory) {
            for (std::pair<int, int> edge : gr[vertex])
                if (visited[vertex] + edge.second < visited[edge.first]) {
                    visited[edge.first] = visited[vertex] + edge.second;
                }
        }

        return visited;
    }

    std::vector<std::vector<int>> get_FloydWarshall() { // поиск расстояний от каждой вершины до каждй вершины (для простых ографов без петель и отрицательных циклов)
        std::vector<std::vector<int>> dist(gr.size(), std::vector<int>(gr.size(), INF)); // матрица-результат, в ней же и будем проводить вычисления

        for (int i = 0; i < gr.size(); ++i) { dist[i][i] = 0; } // заполняем диагональ матрицы 0

        for (int vertex = 0; vertex < gr.size(); ++vertex)
            for (std::pair<int, int> edge : gr[vertex])
                dist[vertex][edge.first] = edge.second;

        for (int v_between = 0; v_between < gr.size(); ++v_between)
            for (int v_from = 0; v_from < gr.size(); ++v_from)
                for (int v_to = 0; v_to < gr.size(); ++v_to)
                    if (dist[v_from][v_between] + dist[v_between][v_to] < dist[v_from][v_to])
                        dist[v_from][v_to] = dist[v_from][v_between] + dist[v_between][v_to];

        std::vector<std::vector<int>> result = dist;

        return result;
    }

    std::vector<std::vector<int>> get_Johnson() {
        visited.assign(gr.size(), 0);
        std::vector<std::list<
            std::pair<int, int>>
            > gr_memory; // вспомогательное хранилище ребер
        std::vector<std::vector<int>> dist(gr.size(), std::vector<int>(gr.size())); // матрица-результат, в ней же и будем проводить вычисления
                                                                                       // инициализиурем все 0, чтобы запустить алгоритм Беллмана-Форда "от вех вершин сразу"
        // Классический вариант алгоритма Беллмана-Форда. Тупо ctrl+c ctrl+v из реализации сверху
        // Решил не мудрить с выноской основной части алгоритма в отдельный приватный метод, чтобы сохранить наглядность 
        // и алгоритма Беллмана-Форда, и алгоритма Джонсона (проще говоря чтобы все в одном месте было)

        bool flag = true;
        while (flag) {
            flag = false;
            for (int vertex = 0; vertex < gr.size(); ++vertex)
                for (std::pair<int, int> edge : gr[vertex])
                    if (visited[vertex] + edge.second < visited[edge.first]) {
                        visited[edge.first] = visited[vertex] + edge.second;
                        flag = true;
                    }
        }

        gr_memory = gr;
        for (int vertex = 0; vertex < gr.size(); ++vertex)
            for (std::pair<int, int>& edge : gr[vertex])
                edge.second = edge.second + visited[vertex] - visited[edge.first];

        std::vector<int> visited_memory = visited;
        for (int vertex = 0; vertex < gr.size(); ++vertex) {
            dist[vertex] = get_Dijkstra(vertex + 1); // запускаем Дейкстру для каждой вершины
            for (int i = 0; i < gr.size(); ++i)
                dist[vertex][i] = dist[vertex][i] - visited_memory[vertex] + visited_memory[i];
        }

        gr = gr_memory;

        std::vector<std::vector<int>> result = dist;

        return result;
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

    Ograph get_inverted_gr() { // построение инвертированного графа
        Ograph inverted_gr(gr.size()); // вспомогательный инверитрованный граф
        for (int i = 0; i < gr.size(); ++i) { // заполняем вспомогательный неориентированный инвертированный граф
            for (std::pair<int, int> edge : gr[i]) {
                inverted_gr.add_edge(edge.first + 1, i + 1);
            }
        }

        return inverted_gr;
    }
};

int main()
{
    Ograph graph;

    //graph.add_edge(1, 2); graph.add_edge(1, 6); graph.add_edge(1, 10);
    //graph.add_edge(2, 3);
    //graph.add_edge(3, 4);
    //graph.add_edge(6, 7); graph.add_edge(6, 8);
    //graph.add_edge(10, 11);
    //graph.add_edge(11, 8);
    //graph.add_edge(4, 7); graph.add_edge(4, 5);
    //graph.add_edge(7, 9);
    //graph.add_edge(8, 9); graph.add_edge(8, 12);
    //graph.add_edge(5, 13);
    //graph.add_edge(9, 13);
    //graph.add_edge(12, 13);

    //graph.add_edge(1, 2, 15);
    //graph.add_edge(2, 3, 25);
    //graph.add_edge(3, 4, 10);
    //graph.add_edge(1, 6, 20);
    //graph.add_edge(2, 7, 0);
    //graph.add_edge(3, 8, 70);
    //graph.add_edge(4, 9, 50);
    //graph.add_edge(5, 10, 10);
    //graph.add_edge(7, 8, 40);
    //graph.add_edge(8, 9, 30);
    //graph.add_edge(6, 11, 30);
    //graph.add_edge(7, 11, 10);
    //graph.add_edge(8, 13, 60);
    //graph.add_edge(9, 13, 5);
    //graph.add_edge(9, 14, 70);
    //graph.add_edge(10, 15, 25);
    //graph.add_edge(11, 12, 10);
    //graph.add_edge(12, 13, 35);
    //graph.add_edge(13, 14, 70);

    //graph.add_edge(1, 2, 1);
    //graph.add_edge(2, 3, 4);
    //graph.add_edge(3, 4, 5);
    //graph.add_edge(5, 1, 7);
    //graph.add_edge(6, 2, 2);
    //graph.add_edge(7, 3, 3);
    //graph.add_edge(4, 8, -3);
    //graph.add_edge(6, 5, 5);
    //graph.add_edge(6, 7, -2);
    //graph.add_edge(7, 8, 7);
    //graph.add_edge(9, 5, -6);
    //graph.add_edge(6, 10, 4);
    //graph.add_edge(7, 11, 6);
    //graph.add_edge(8, 12, 4);
    //graph.add_edge(10, 9, 3);
    //graph.add_edge(10, 11, 1);
    //graph.add_edge(11, 12, 2);

    graph.add_edge(1, 2, 5);
    graph.add_edge(2, 3, 5);
    graph.add_edge(2, 4, 3);
    graph.add_edge(3, 1, -3);
    graph.add_edge(4, 1, 2);
    graph.add_edge(4, 3, -5);

    //graph.add_edge(1, 2, 3);
    //graph.add_edge(1, 3, 10);
    //graph.add_edge(3, 2, -10);
    //graph.add_edge(2, 4, 5);

    std::vector<std::vector<int>> result = graph.get_Johnson();


    for (int i = 0; i < result[0].size(); ++i) {
        for (int j = 0; j < result[0].size(); ++j) {
            std::cout << result[i][j] << " ";
        }
        std::cout << "\n";
    }
}