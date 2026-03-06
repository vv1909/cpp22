#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <vector>

#define N 8

using namespace std;

int cx[8] = {1, 1, 2, 2, -1, -1, -2, -2};
int cy[8] = {2, -2, 1, -1, 2, -2, 1, -1};



// Count available moves from (x,y)
int calculateDegree(int board[N][N], int x, int y) {
    int count = 0;
    for (int i = 0; i < 8; i++) {
        int nx = x + cx[i];
        int ny = y + cy[i];
        if (nx >= 0 && ny >= 0 && nx < N && ny < N && board[ny][nx] == -1)
            count++;
    }
    return count;
}

// Update entire degree matrix
void updateDegreeMatrix(int board[N][N], int degree[N][N]) {
    for (int y = 0; y < N; y++)
        for (int x = 0; x < N; x++) {
            if (board[y][x] == -1)
                degree[y][x] = calculateDegree(board, x, y);
            else
                degree[y][x] = -1; // already visited
        }
}

// Choose next move using degree matrix, with random tie‑breaking
bool nextMove(int board[N][N], int degree[N][N], int &x, int &y) {
    // Refresh degree matrix
    updateDegreeMatrix(board, degree);

    int min_deg = 9;
    vector<pair<int, int>> candidates; // stores all moves with minimal degree

    for (int i = 0; i < 8; i++) {
        int tx = x + cx[i];
        int ty = y + cy[i];

        if (tx >= 0 && ty >= 0 && tx < N && ty < N && board[ty][tx] == -1) {
            int d = degree[ty][tx];
            if (d < min_deg) {
                min_deg = d;
                candidates.clear();
                candidates.emplace_back(tx, ty);
            } else if (d == min_deg) {
                candidates.emplace_back(tx, ty);
            }
        }
    }

    if (candidates.empty())
        return false; // no legal move

    // Randomly choose one of the best candidates
    int idx = rand() % candidates.size();
    int nx = candidates[idx].first;
    int ny = candidates[idx].second;

    // Place the next move number
    board[ny][nx] = board[y][x] + 1;
    x = nx;
    y = ny;

    return true;
}

// Print board
void print(int board[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++)
            cout << setw(3) << board[i][j] << " ";
        cout << endl;
    }
}

// Check if the last square is a knight's move away from the start (closed tour)
bool neighbour(int x, int y, int sx, int sy) {
    for (int i = 0; i < 8; i++)
        if (x + cx[i] == sx && y + cy[i] == sy)
            return true;
    return false;
}

// Main solver – tries to find a closed tour from (sx, sy)
bool findClosedTour(int sx, int sy) {
    int board[N][N];
    int degree[N][N];

    // Initialize board with -1 (unvisited)
    for (int y = 0; y < N; y++)
        for (int x = 0; x < N; x++)
            board[y][x] = -1;

    int x = sx, y = sy;
    board[y][x] = 1; // start with move 1

    // Make the remaining 63 moves
    for (int i = 0; i < N * N - 1; i++)
        if (!nextMove(board, degree, x, y))
            return false; // stuck

    // Verify that the last square connects back to the start
    if (!neighbour(x, y, sx, sy))
        return false;

    print(board);
    return true;
}

int main() {
    srand(time(nullptr)); // seed random generator once

    int sx, sy;
    cout << "Insert starting position (0-7 0-7): ";
    cin >> sx >> sy;

    // Keep trying until a closed tour is found
    while (!findClosedTour(sx, sy)) {
        // Uncomment the next line to see progress (optional)
        // cout << "Attempt failed, retrying..." << endl;
    }

    return 0;
}