#include "header.hpp"
#include <iostream>
#include <queue>
#include <cstdlib>
#include <ctime>

using namespace std;

int arrx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
int arry[] = {-1, 0, 1, -1, 1, -1, 0, 1};
int n = 20, m = 20, cnt = 60, **arr = nullptr, **showarr = nullptr;

int **initarr(int **arr, int n, int m) {
    arr = (int **)calloc(n + 2, sizeof(int *));
    for (int i = 0; i < n + 2; i++) {
        arr[i] = (int *)calloc(m + 2, sizeof(int));
    }
    return arr;
}

bool check(int x, int y) {
    return (x >= 0 && x < n + 2 && y >= 0 && y < m + 2);
}

void bfstraversal(int **showarr, int **arr, int x, int y) {
    queue<pair<int, int>> q;
    q.push({x, y});
    
    while (!q.empty()) {
        int a = q.front().first;
        int b = q.front().second;
        showarr[a][b] = arr[a][b];
        q.pop();

        for (int i = 0; i < 8; i++) {
            int nx = a + arrx[i];
            int ny = b + arry[i];
            if (check(nx, ny) && showarr[nx][ny] == 10 && arr[a][b] == 0) {
                q.push({nx, ny});
            }
        }
    }
}

void generate(int **arr, int n, int m, int k) {
    srand(time(NULL));
    while (k > 0) {
        int x = rand() % n + 1;
        int y = rand() % m + 1;
        if (arr[x][y] != 9) {
            arr[x][y] = 9;
            k--;
        }
    }
}

void count(int **arr, int **showarr, int n, int m) {
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            showarr[i][j] = 10; // Unopened cells
            if (arr[i][j] != 9) {
                for (int q = 0; q < 8; q++) {
                    if (check(i + arrx[q], j + arry[q]) && arr[i + arrx[q]][j + arry[q]] == 9) {
                        arr[i][j]++;
                    }
                }
            }
        }
    }
}

void exchange(int **arr, int x, int y, int i, int j) {
    for (int q = 0; q < 8; q++) {
        if (check(x + arrx[q], y + arry[q]) && arr[x + arrx[q]][y + arry[q]] != 9) {
            arr[x + arrx[q]][y + arry[q]]--;
        }
        if (check(i + arrx[q], j + arry[q]) && arr[i + arrx[q]][j + arry[q]] != 9) {
            arr[i + arrx[q]][j + arry[q]]++;
        }
    }
}

void newcnt(int **arr, int x, int y) {
    arr[x][y] = 0;
    for (int q = 0; q < 8; q++) {
        if (check(x + arrx[q], y + arry[q]) && arr[x + arrx[q]][y + arry[q]] == 9) {
            arr[x][y]++;
        }
    }
}

void firstmove(int **arr, int x, int y, int n, int m) {
    if (arr[x][y] == 9) {
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= m; j++) {
                if (arr[i][j] != 9) {
                    exchange(arr, x, y, i, j);
                    newcnt(arr, x, y);
                    arr[i][j] = 9;
                    return;
                }
            }
        }
    }
}

// Remember to free the allocated memory when done

