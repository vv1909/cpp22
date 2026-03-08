#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
using namespace std;

const int n = 8;
int board[n][n];
int degree[n][n];

int dir[8][2] = { {2, 1}, {1, 2}, {-1, 2}, {-2, 1},
                  {-2, -1}, {-1, -2}, {1, -2}, {2, -1} };

// Initialise degree matrix: number of legal moves from each cell
// when all cells are empty (only board boundaries considered).
void initDegree() {
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
void updateDegree(int x, int y, int delta) {
    for (int d = 0; d < 8; d++) {
        int nx = x + dir[d][0];
        int ny = y + dir[d][1];
        if (nx >= 0 && nx < n && ny >= 0 && ny < n && board[nx][ny] == -1) {
            degree[nx][ny] += delta;
        }
    }
}

// Returns the number of unvisited neighbour moves from (x,y).
// Fills moveDirs with the direction indices (0..7) and moveDegrees with
// the current degree of each neighbour.
int getMoves(int x, int y, int moveDirs[8], int moveDegrees[8]) {
    int cnt = 0;
    for (int i = 0; i < 8; i++) {
        int nx = x + dir[i][0];
        int ny = y + dir[i][1];
        if (nx >= 0 && nx < n && ny >= 0 && ny < n && board[nx][ny] == -1) {
            moveDirs[cnt] = i;
            moveDegrees[cnt] = degree[nx][ny];
            cnt++;
        }
    }
    return cnt;
}

bool knightTour(int x, int y, int step) {
    if (step == n * n)
        return true;

    int moveDirs[8];
    int moveDegrees[8];
    int moveCount = getMoves(x, y, moveDirs, moveDegrees);

    if (moveCount == 0)
        return false;

    // Find the minimum degree among the moves
    int minDegree = moveDegrees[0];
    for (int i = 1; i < moveCount; i++)
        if (moveDegrees[i] < minDegree)
            minDegree = moveDegrees[i];

    // Collect indices of moves with that minimum degree
    int candidates[8];
    int candCount = 0;
    for (int i = 0; i < moveCount; i++) {
        if (moveDegrees[i] == minDegree) {
            candidates[candCount++] = i;
        }
    }

    // Shuffle the candidates randomly
    for (int i = 0; i < candCount - 1; i++) {
        int r = i + rand() % (candCount - i);
        // swap candidates[i] and candidates[r]
        int temp = candidates[i];
        candidates[i] = candidates[r];
        candidates[r] = temp;
    }

    // Try each candidate in the shuffled order
    for (int i = 0; i < candCount; i++) {
        int dirIdx = moveDirs[candidates[i]];
        int nx = x + dir[dirIdx][0];
        int ny = y + dir[dirIdx][1];

        board[nx][ny] = step;
        updateDegree(nx, ny, -1);

        if (knightTour(nx, ny, step + 1))
            return true;

        // Backtrack
        updateDegree(nx, ny, +1);
        board[nx][ny] = -1;
    }

    return false;
}

int main() {
    srand(static_cast<unsigned>(time(nullptr)));

    int startx, starty;
    cout << "Input starting position (0-7) (0-7): ";
    cin >> startx >> starty;

    // Initialise board and degree matrix
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            board[i][j] = -1;
    initDegree();

    board[startx][starty] = 0;
    updateDegree(startx, starty, -1);

    if (knightTour(startx, starty, 1)) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++)
                cout << setw(4) << board[i][j];
            cout << endl << endl;
        }
    } else {
        cout << "no solution" << endl;
    }
    return 0;
}
