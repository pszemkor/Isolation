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

vector<pair<int, int>> read_park(char **park, set<pair<int, int>> &crossings);

void print_park(char *const *park);

bool place_citizens(int l, char **park, const vector<pair<int, int>> &points, set<pair<int, int>> crossings);

bool can_be_placed(int row, int col, int l, char **park, set<pair<int, int>> marked);


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

    char str[2500];
    scanf("%2499[^\n]", str);
    string map_name = str;
    flush();

    char **park = new char *[H];
    set<pair<int, int>> crossings;
    vector<pair<int, int>> path_points = read_park(park, crossings);
//    print_park(park);
    bool res = place_citizens(L, park, path_points, crossings);
    if (res) {
        cout << "SUCCESS" << endl;
    }
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

bool place_citizens(int l, char **park, const vector<pair<int, int>> &points, set<pair<int, int>> crossings) {
//    set<pair<int, int>> last_marked;
//    for (auto point: points) {
    set<pair<int, int>> marked;
    cout << crossings.size() << endl;
//    marked.insert(point);
    for (auto p: points) {
        cout << "ENDPOINTS: " << p.first << " " << p.second << endl;
        if (can_be_placed(p.first, p.second, l, park, marked)) {
            marked.insert(p);
        }
    }
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            if (park[i][j] == '.') {
                continue;
            }
            pair<int, int> p = make_pair(i, j);
            if (crossings.find(p) != crossings.end()) {
                continue;
            }
            bool is_available = can_be_placed(i, j, l, park, marked);

            if (is_available) {
                marked.insert(p);
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
//    }

    print_solution(marked);
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

vector<pair<int, int>> read_park(char **park, set<pair<int, int>> &crossings) {
    vector<pair<int, int>> tmp_points;
    set<pair<int, int>> tmp;
    int shift[] = {-1, 1};

    for (int i = 0; i < H; i++) {
        park[i] = new char[W];
    }
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            cin >> park[i][j];
            if (is_path(i, j, park)) {
                tmp_points.emplace_back(make_pair(i, j));
            }
            if (park[i][j] == '+') {
                tmp.insert(make_pair(i, j));
            }
        }
    }
    flush();
    for (auto c: tmp) {
        int row = c.first;
        int col = c.second;
        int counter = 0;
        for (int i : shift) {
            int shifted_row = row + i;
            int shifted_col = col + i;
            if (is_in_park(shifted_row, col) && (park[shifted_row][col] == '|' || park[shifted_row][col] == '+')) {
                ++counter;
            }
            if (is_in_park(row, shifted_col) && (park[row][shifted_col] == '-' || park[row][shifted_col] == '+')) {
                ++counter;
            }
        }
        if (counter >= 2) {
            crossings.insert(c);
        }

    }

    vector<pair<int, int>> paths_points;
    for (auto p: tmp_points) {
        int row = p.first;
        int col = p.second;
        if (park[row][col] == '|') {
            if (!is_in_park(row + 1, col) or !is_in_park(row - 1, col) or !is_path(row + 1, col, park) or
                !is_path(row - 1, col, park)) {
                paths_points.emplace_back(p);
            } else if (is_in_park(row + 1, col) && park[row + 1][col] == '-') {
                paths_points.emplace_back(p);
            } else if (is_in_park(row - 1, col) && park[row - 1][col] == '-') {
                paths_points.emplace_back(p);
            }
        }
        if (park[row][col] == '-') {
            if (!is_in_park(row, col + 1) or !is_in_park(row, col - 1) or !is_path(row, col + 1, park) or
                !is_path(row, col - 1, park)) {
                paths_points.emplace_back(p);
            } else if (is_in_park(row, col + 1) && park[row][col + 1] == '|') {
                paths_points.emplace_back(p);
            } else if (is_in_park(row, col - 1) && park[row][col - 1] == '|') {
                paths_points.emplace_back(p);
            }
        }


    }


    return tmp_points;
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