#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <map>
#include <set>
#include<list>

int W = -1;
int H = -1;
int L = -1;
int K = -1;
using namespace std;

vector<pair<int, int>> read_park(char **park);

void print_park(char *const *park);

bool place_citizens(int l, char **park, vector<pair<int, int>> set);

bool can_be_placed(int row, int col, int l, char **park, set<pair<int, int>> marked);

void subset(vector<pair<int, int>> arr, int size, int left, int index, list<pair<int, int>> &l, char **park);

void print_solution(const set<pair<int, int>> marked);

bool can_move_horizontal(int row, int c, char **park);

bool can_move_vertical(int row, int c, char **park);

void flush() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main() {
    scanf("%i", &W);
    scanf("%i", &H);
    scanf("%i", &L);
    scanf("%i", &K);
    flush();

    char str[250];
    scanf("%249[^\n]", str);
    string map_name = str;
    flush();

    char **park = new char *[H];
    vector<pair<int, int>> path_points = read_park(park);
    place_citizens(L, park, path_points);

    return 0;
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

bool place_citizens(int l, char **park, vector<pair<int, int>> points) {
    set<pair<int, int>> last_marked;
    for (auto point: points) {
        set<pair<int, int>> marked;
        set<pair<int, int>> crossings;
        marked.insert(point);
        for (int i = 0; i < H; i++) {
            for (int j = 0; j < W; j++) {
                if (park[i][j] == '.') {
                    continue;
                }
                if (park[i][j] == '+') {
                    crossings.insert(make_pair(i, j));
                    continue;
                }
                bool is_available = can_be_placed(i, j, l, park, marked);
                if (is_available) {
                    marked.insert(make_pair(i, j));
                }
                if (marked.size() >= K) {
                    print_solution(marked);
                    return true;
                }
            }
        }

        for (auto p : crossings) {
            if (can_be_placed(p.first, p.second, l, park, marked)) {
                marked.insert(p);
            }
        }
        if (marked.size() >= K) {
            print_solution(marked);
            return true;
        }
        last_marked = marked;
    }

    print_solution(last_marked);

    return false;
}

void print_solution(const set<pair<int, int>> marked) {
    int so_far = 0;
    for (auto p : marked) {
        printf("%d %d\n", p.second, p.first);
        so_far++;
        if (so_far == K) {
            break;
        }
    }
}


void print_park(char *const *park) {
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            cout << park[i][j] << ", ";
        }
        cout << endl;
    }
}

vector<pair<int, int>> read_park(char **park) {
    vector<pair<int, int>> paths_points;
    for (int i = 0; i < H; i++) {
        park[i] = new char[W];
    }
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            cin >> park[i][j];
            if (is_path(i, j, park)) {
                paths_points.emplace_back(make_pair(i, j));
            }
        }
    }
    flush();
    return paths_points;
}

bool can_be_placed(int row, int col, int l, char **park, set<pair<int, int>> marked) {
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
        if (dist[current] <= l && marked.find(current) != marked.end()) {
//            cout << "TURNED DOWN: " << src.first << " " << src.second << endl;
            return false;
        }
        if (dist[current] == l) {
            continue;
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
    return true;
}
