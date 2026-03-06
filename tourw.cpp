#include <iostream>
#include <algorithm>
#include <iomanip>
#include <random>
#include <chrono>
using namespace std;

int dir[8][2] = { {2, 1}, {1, 2}, {-1, 2}, {-2, 1},
                  {-2, -1}, {-1, -2}, {1, -2}, {2, -1} };

// Initialise degree matrix: number of legal moves from each cell
// when all cells are empty (only board boundaries considered).
void initDegree(int degree[8][8], int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int cnt = 0;
            for (int d = 0; d < 8; d++) {
                int ni = i + dir[d][0];
                int nj = j + dir[d][1];
                if (ni >= 0 && ni < n && nj >= 0 && nj < n)
                    cnt++;
            }
            degree[i][j] = cnt;
        }
    }
}

// Update degree matrix when a knight is placed on (x,y) (delta = -1)
// or removed from (x,y) (delta = +1).
void updateDegree(int board[8][8], int degree[8][8], int n, int x, int y, int delta) {
    for (int d = 0; d < 8; d++) {
        int nx = x + dir[d][0];
        int ny = y + dir[d][1];
        if (nx >= 0 && nx < n && ny >= 0 && ny < n && board[nx][ny] == -1) {
            degree[nx][ny] += delta;
        }
    }
}

// Fills moveDirs and moveDegrees with all unvisited neighbours of (x,y).
// Returns the number of valid moves (≤8). The arrays are sorted by degree
// (ascending), and within equal degrees they are randomly shuffled.
int getSortedMoves(int board[8][8], int degree[8][8], int n,
                   int x, int y, int moveDirs[8], int moveDegrees[8]) {
    int moveCount = 0;
    for (int i = 0; i < 8; i++) {
        int nx = x + dir[i][0];
        int ny = y + dir[i][1];
        if (nx >= 0 && nx < n && ny >= 0 && ny < n && board[nx][ny] == -1) {
            moveDirs[moveCount] = i;
            moveDegrees[moveCount] = degree[nx][ny];
            moveCount++;
        }
    }

    // Sort moves by degree (ascending) using simple selection sort
    // because moveCount ≤ 8.
    for (int i = 0; i < moveCount - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < moveCount; j++) {
            if (moveDegrees[j] < moveDegrees[minIdx])
                minIdx = j;
        }
        if (minIdx != i) {
            swap(moveDegrees[i], moveDegrees[minIdx]);
            swap(moveDirs[i], moveDirs[minIdx]);
        }
    }

    // Random generator (static to keep state between calls)
    static random_device rd;
    static mt19937 g(rd());

    // Shuffle each group of equal degree
    int i = 0;
    while (i < moveCount) {
        int currentDeg = moveDegrees[i];
        int j = i;
        while (j < moveCount && moveDegrees[j] == currentDeg)
            j++;
        // Shuffle the directions in the range [i, j)
        shuffle(moveDirs + i, moveDirs + j, g);
        i = j;
    }

    return moveCount;
}

bool knightTour(int board[8][8], int degree[8][8], int n,
                int x, int y, int step) {
    if (step == n * n)
        return true;

    int moveDirs[8];
    int moveDegrees[8];
    int moveCount = getSortedMoves(board, degree, n, x, y, moveDirs, moveDegrees);

    if (moveCount == 0)
        return false;

    for (int i = 0; i < moveCount; i++) {
        int dirIdx = moveDirs[i];
        int nx = x + dir[dirIdx][0];
        int ny = y + dir[dirIdx][1];

        board[nx][ny] = step;
        updateDegree(board, degree, n, nx, ny, -1);   // occupy the cell

        if (knightTour(board, degree, n, nx, ny, step + 1))
            return true;

        // Backtrack
        updateDegree(board, degree, n, nx, ny, +1);   // free the cell
        board[nx][ny] = -1;
    }

    return false;
}

int main() {
    int n = 8;
    int startx, starty;
    cout << "Input coordinates (0-7) (0-7): ";
    cin >> startx >> starty;

    int board[8][8];
    int degree[8][8];

    // Initialise board and degree matrix
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            board[i][j] = -1; 
    initDegree(degree, n);

    // Place the starting knight and update degree accordingly
    board[startx][starty] = 0;
    updateDegree(board, degree, n, startx, starty, -1);

    if (knightTour(board, degree, n, startx, starty, 1)) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++)
                cout << setw(4) << board[i][j];
            cout << endl;
        }
    } else {
        cout << "no solution" << endl;
    }
    return 0;
}