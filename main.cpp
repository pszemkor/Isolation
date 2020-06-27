#include <iostream>
#include <vector>
#include <queue>
#include <string>

int W = -1;
int H = -1;

void read_park(char **park);

void print_park(char *const *park);

int **parse_park(int w, int h, char **park);

void place_citizens(int l, char **park);

using namespace std;


int get_node_name(int row, int col) {
    return row * H + col;
}


int get_col(int node_name) {
    return node_name % H;
}

int get_row(int node_name, int col) {
    return (node_name - col) / H;
}

void flush() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main() {

    W = 5;
    H = 5;
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            int n = get_node_name(i, j);
            cout << n << endl;
            int col = get_col(n);
            cout << col << j << endl;
            cout << get_row(n, col) << i << endl;
            cout << "*************" << endl;
        }
    }
//    // read W, H, L, K
//    int L;
//    int K;
//    scanf("%i", &W);
//    scanf("%i", &H);
//    scanf("%i", &L);
//    scanf("%i", &K);
//    cout << W << H << L << K << endl;
//    flush();
//    char str[25];
//    scanf("%24[^\n]", str);
//    string map_name = str;
//    cout << map_name << endl;
//    flush();
//    char **park = new char *[H];
//    read_park(park);
//    print_park(park);
//    place_citizens(L, park);
//

    return 0;
}

bool is_path(int x, int y, char **park) {
    return park[x][y] != '.';
}

bool is_in_park(int row, int col) {
    return row >= 0 && row < H && col >= 0 && col < W;
}

bool has_marked_l_neighbour(int row, int col, int l, char **park, const bool *marked) {
    queue<int> q;
    int src = get_node_name(row, col);
    int *dist;
    int *parent;
    bool *visited;
    dist = new int[W * H];
    parent = new int[W * H];
    visited = new bool[W * H];
    for (int i = 0; i < W * H; i++) {
        dist[i] = INT_MAX;
        visited[i] = false;
        parent[i] = -1;
    }
    q.push(src);
    visited[src] = true;
    dist[src] = 0;
    while (!q.empty()) {
        int current = q.front();
        q.pop();
        if (dist[current] == l && marked[current]) {
            return false;
        }
        if (dist[current] == l) {
            continue;
        }
        col = get_col(current);
        row = get_row(current, col);
        int shift[] = {-1, 1};
        if (park[row][col] == '-') {
            for (int i = 0; i < 2; i++) {
                int c_c = col + shift[i];
                int neighbour = get_node_name(row, c_c);
                if (is_in_park(row, c_c) && is_path(row, c_c, park) && !visited[neighbour]) {
                    q.push(neighbour);
                    parent[neighbour] = current;
                    dist[neighbour] = dist[current] + 1;
                    visited[neighbour] = true;
                }
            }
        } else if (park[row][col] == '|') {
            for (int i = 0; i < 2; i++) {
                int c_r = row + shift[i];
                int neighbour = get_node_name(c_r, col);
                if (is_in_park(c_r, col) && is_path(c_r, col, park) && !visited[neighbour]) {
                    q.push(neighbour);
                    parent[neighbour] = current;
                    dist[neighbour] = dist[current] + 1;
                    visited[neighbour] = true;
                }
            }
        } else if (park[row][col] == '+') {
            int row_shift[] = {-1, 1, 0, 0};
            int col_shift[] = {0, 0, -1, 1};
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    int c_r = row + row_shift[i];
                    int c_c = col + col_shift[j];
                    int neighbour = get_node_name(c_r, c_c);
                    if (is_in_park(c_r, c_c) && is_path(c_r, c_c, park) && !visited[neighbour]) {
                        q.push(neighbour);
                        parent[neighbour] = current;
                        dist[neighbour] = dist[current] + 1;
                        visited[neighbour] = true;
                    }
                }
            }
        }
    }
    return true;
}

void place_citizens(int l, char **park) {
    bool *marked;
    marked = new bool[W * H];
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            if (park[i][j] == '.') {
                continue;
            }
            bool is_available = has_marked_l_neighbour(i, j, l, park, marked);
            if (is_available) {
                marked[get_node_name(i, j)] = true;
                cout << i << "  " << j << endl;
            }
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

void read_park(char **park) {
    for (int i = 0; i < H; i++) {
        park[i] = new char[W];
    }
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            cin >> park[i][j];
        }
    }
}
