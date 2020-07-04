#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <map>
#include <set>
#include<list>

// graph representation based on:
//https://www-users.mat.umk.pl/~stencel/acm/algorytmika_praktyczna.pdf
int W = -1;
int H = -1;
int L = -1;
int K = -1;
using namespace std;


struct Graph {
    struct V : set<pair<int, int> > {
    };
    map<pair<int, int>, V> g;
    map<pair<int, int>, V> extendedGraph;


    Graph() = default;

    void addEdge(const pair<int, int> b, const pair<int, int> e) {
        init(b);
        init(e);
        g[b].insert(b);
        g[e].insert(e);
    }

    void addExtendedEdge(pair<int, int> b, pair<int, int> e) {
        initExtended(b);
        initExtended(e);
        extendedGraph[b].insert(e);
        extendedGraph[e].insert(b);
    }

    void removeExtendedEdge(pair<int, int> b, pair<int, int> e) {
        extendedGraph[b].erase(e);
        extendedGraph[e].erase(b);
    }

    void removeFromExtendedGraph(pair<int, int> v) {
        extendedGraph.erase(v);
    }

    void initExtended(const pair<int, int> e) {
        if (extendedGraph.find(e) == extendedGraph.end()) {
            V v;
            extendedGraph.insert(make_pair(e, v));
        }
    }

private:
    void init(const pair<int, int> e) {
        if (g.find(e) == g.end()) {
            V v;
            g.insert(make_pair(e, v));
        }
    }
};


char **read_park();

void print_park(char *const *park);

set<pair<int, int>> place_citizens(Graph park);

void print_solution(const set<pair<int, int>> &marked);

bool can_move_horizontal(int row, int c, char **park);

bool can_move_vertical(int row, int c, char **park);

void count_fields(int row, int col, char **park, set<pair<int, int>> &extended_neighbours);

set<pair<int, int>> get_neighbours(int row, int col, char **park);

Graph build_graph(char **park);

void flush();

int main() {
    scanf("%i", &W);
    scanf("%i", &H);
    scanf("%i", &L);
    scanf("%i", &K);
    flush();

    char str[2500];
    scanf("%2499[^\n]", str);
    string map_name = str;
    flush();

    set<pair<int, int>> crossings;
    char **park = read_park();
//    print_park(park);
    Graph g = build_graph(park);
    set<pair<int, int>> selected = place_citizens(g);
    if (selected.size() == K) {
        print_solution(selected);
    }

    return 0;
}

Graph build_graph(char **park) {
    Graph g = Graph();
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            if (park[i][j] == '.') {
                continue;
            }
            set<pair<int, int>> neigh = get_neighbours(i, j, park);
            set<pair<int, int>> extended;
            count_fields(i, j, park, extended);
            pair<int, int> current = make_pair(i, j);
            g.initExtended(current);
            for (pair<int, int> node: neigh) {
                g.addEdge(current, node);
            }
            /*
             Extended edges are responsible for storing nodes that are L
             */
            for (pair<int, int> node: extended) {
                g.addExtendedEdge(current, node);
            }

        }
    }
    return g;
}

bool is_path(int x, int y, char **park) {
    return park[x][y] != '.';
}

bool is_in_park(int row, int col) {
    return row >= 0 && row < H && col >= 0 && col < W;
}

bool can_move_horizontal(int row, int c, char **park) {
    return park[row][c] == '+' || park[row][c] == '-';
}

bool can_move_vertical(int row, int c, char **park) {
    return park[row][c] == '+' || park[row][c] == '|';
}

set<pair<int, int>> place_citizens(Graph graph) {
    set<pair<int, int>> marked;
    while (!graph.extendedGraph.empty()) {
        int min = W * H * 2;
        pair<int, int> optimal;
        for (const pair<const pair<int, int>, Graph::V> &node: graph.extendedGraph) {
            int size = node.second.size();
            if (min > size) {
                min = size;
                optimal = node.first;
            }
        }
        marked.insert(optimal);
        //drop nodes that cannot be used in further processing
        Graph::V neighs = graph.extendedGraph[optimal];
        for (pair<int, int> neighbour: neighs) {
            // neighbours of optimal's neighbour
            Graph::V nns = graph.extendedGraph[neighbour];
            for (pair<int, int> nn: nns) {
                graph.removeExtendedEdge(nn, neighbour);
            }
            graph.removeFromExtendedGraph(neighbour);
        }
        graph.removeFromExtendedGraph(optimal);
    }

    return marked;
}

void print_solution(const set<pair<int, int>> &marked) {
    int so_far = 0;
    for (auto p : marked) {
        printf("%d %d\n", p.second, p.first);
        so_far++;
        if (so_far == K) {
            break;
        }
    }
}

void count_fields(int row, int col, char **park, set<pair<int, int>> &extended_neighbours) {
    queue<pair<int, int>> q;
    pair<int, int> src = make_pair(row, col);
    map<pair<int, int>, int> dist;
    map<pair<int, int>, pair<int, int>> parent;
    set<pair<int, int>> visited;
    q.push(src);
    visited.insert(src);
    dist[src] = 0;
    while (!q.empty()) {
        pair<int, int> current = q.front();
        q.pop();
        if (dist[current] == L + 1) {
            continue;
        }
        if (current != src) {
            extended_neighbours.insert(current);
        }
        col = current.second;
        row = current.first;
        int shift[] = {-1, 1};
        if (park[row][col] == '-') {
            for (int i = 0; i < 2; i++) {
                int c_c = col + shift[i];
                pair<int, int> neighbour = make_pair(row, c_c);
                if (is_in_park(row, c_c) && is_path(row, c_c, park) && visited.find(neighbour) == visited.end() &&
                    can_move_horizontal(row, c_c, park)) {
                    q.push(neighbour);
                    parent[neighbour] = current;
                    dist[neighbour] = dist[current] + 1;
                    visited.insert(neighbour);
                }
            }
        } else if (park[row][col] == '|') {
            for (int i = 0; i < 2; i++) {
                int c_r = row + shift[i];
                pair<int, int> neighbour = make_pair(c_r, col);
                if (is_in_park(c_r, col) && is_path(c_r, col, park) && visited.find(neighbour) == visited.end() &&
                    can_move_vertical(c_r, col, park)) {
                    q.push(neighbour);
                    parent[neighbour] = current;
                    dist[neighbour] = dist[current] + 1;
                    visited.insert(neighbour);
                }
            }
        } else if (park[row][col] == '+') {
            int row_shift[] = {-1, 1, 0, 0};
            int col_shift[] = {0, 0, -1, 1};
            for (int i = 0; i < 2; i++) {
                for (int j = 0; j < 2; j++) {
                    int c_r = row + row_shift[i];
                    int c_c = col + col_shift[j];
                    pair<int, int> neighbour = make_pair(c_r, c_c);
                    if (is_in_park(c_r, c_c) && is_path(c_r, c_c, park) && visited.find(neighbour) == visited.end() &&
                        can_move_vertical(c_r, c_c, park)) {
                        q.push(neighbour);
                        parent[neighbour] = current;
                        dist[neighbour] = dist[current] + 1;
                        visited.insert(neighbour);
                    }
                }
            }
            for (int i = 2; i < 4; i++) {
                for (int j = 2; j < 4; j++) {
                    int c_r = row + row_shift[i];
                    int c_c = col + col_shift[j];
                    pair<int, int> neighbour = make_pair(c_r, c_c);
                    if (is_in_park(c_r, c_c) && is_path(c_r, c_c, park) && visited.find(neighbour) == visited.end() &&
                        can_move_horizontal(c_r, c_c, park)) {
                        q.push(neighbour);
                        parent[neighbour] = current;
                        dist[neighbour] = dist[current] + 1;
                        visited.insert(neighbour);
                    }
                }
            }
        }
    }
}

set<pair<int, int>> get_neighbours(int row, int col, char **park) {
    int degree = 0;
    set<pair<int, int>> neighbours;
    int shift[] = {-1, 1};
    char current_node = park[row][col];
    if (current_node == '-' || current_node == '+') {
        for (int sh: shift) {
            int sh_col = col + sh;
            if (is_in_park(row, sh_col) && is_path(row, sh_col, park) && park[row][sh_col] != '|') {
                ++degree;
                neighbours.insert(make_pair(row, sh_col));
            }
        }
    }
    if (current_node == '|' || current_node == '+') {
        for (int sh: shift) {
            int sh_row = row + sh;
            if (is_in_park(sh_row, col) && is_path(sh_row, col, park) && park[sh_row][col] != '-') {
                ++degree;
                neighbours.insert(make_pair(sh_row, col));

            }
        }
    }
    return neighbours;
}

char **read_park() {
    char **park = new char *[H];
    for (int i = 0; i < H; i++) {
        park[i] = new char[W];
    }

    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            cin >> park[i][j];
        }
    }
    flush();
    return park;
}

void print_park(char *const *park) {
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            cout << park[i][j] << ", ";
        }
        cout << endl;
    }
}

void flush() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}