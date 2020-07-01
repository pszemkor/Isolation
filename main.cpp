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

void read_park(char **park, set<pair<int, int>> &crossings);

void print_park(char *const *park);

bool place_citizens(char **park, map<pair<int, int>, int> occurrences, set<pair<int, int>> crossings);

bool can_be_placed(int row, int col, char **park, set<pair<int, int>> marked);

void print_solution(const set<pair<int, int>> marked);

bool can_move_horizontal(int row, int c, char **park);

bool can_move_vertical(int row, int c, char **park);

map<pair<int, int>, int> count_reached_fields(char **pString);

void count_fields(int row, int col, char **park, int *count);

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
    read_park(park, crossings);
    map<pair<int, int>, int> occurrences = count_reached_fields(park);
//    print_park(park);
    bool res = place_citizens(park, occurrences, crossings);
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

map<pair<int, int>, int> count_reached_fields(char **park) {
    map<pair<int, int>, int> occurrences;
    for (int row = 0; row < H; row++) {
        for (int col = 0; col < W; col++) {
            if (is_path(row, col, park)) {
                int count = 0;
                count_fields(row, col, park, &count);
                occurrences[make_pair(row, col)] = count;
            }
        }
    }
    return occurrences;
}

void count_fields(int row, int col, char **park, int *count) {
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
        (*count)++;
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


bool can_move_horizontal(int row, int c, char **park) {
    return park[row][c] == '+' || park[row][c] == '-';
}

bool can_move_vertical(int row, int c, char **park) {
    return park[row][c] == '+' || park[row][c] == '|';
}

bool place_citizens(char **park, map<pair<int, int>, int> occurrences, set<pair<int, int>> crossings) {
    map<pair<int, int>, int>::iterator it;
    set<pair<int, int>> marked;
    int size = occurrences.size();
    for (int i = 0; i < size; i++) {
        int min = W * H + 1;
        pair<int, int> p;
        for (it = occurrences.begin(); it != occurrences.end(); it++) {
            if (it->second < min) {
                min = it->second;
                p = it->first;
            }
        }
        if (can_be_placed(p.first, p.second, park, marked)) {
            marked.insert(p);
//            cout << "row: " << p.first << " col: " << p.second << " :" << park[p.first][p.second] << " reached: "
//                 << occurrences[p] << endl;
        }

        occurrences.erase(p);
//        cout << i << endl;
    }
    if (marked.size() >= K) {
        print_solution(marked);
        return true;
    } else {
        print_solution(marked);
    }
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

void read_park(char **park, set<pair<int, int>> &crossings) {
    vector<pair<int, int>> tmp_points;
    set<pair<int, int>> tmp;

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
}

bool can_be_placed(int row, int col, char **park, set<pair<int, int>> marked) {
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
        if (dist[current] <= L && marked.find(current) != marked.end()) {
//            cout << "TURNED DOWN: " << src.first << " " << src.second << endl;
            return false;
        }
        if (dist[current] == K) {
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