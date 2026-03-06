#include <iostream>
#include <iomanip>

using namespace std;

int N = 8;


int dx[8] = {2, 1, -1, -2, -2, -1, 1, 2};
int dy[8] = {1, 2, 2, 1, -1, -2, -2, -1};

int board[8][8];

bool isPossible(int x, int y) {
    if (x >= 0 && x < N && y >= 0 && y < N && board[x][y] == -1)
        return true;
    return false;
}

bool knight(int x, int y, int step) {
    if (step == N * N)
        return true;

    for (int i = 0; i < 8; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];

        if (isPossible(nx, ny)) {
            board[nx][ny] = step;

            if (knight(nx, ny, step + 1))
                return true;

            board[nx][ny] = -1;
        }
    }
    return false;
}

int main() {
    int startX, startY;

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            board[i][j] = -1;

    cout << "starting X (0-7) and y (0-7): ";
    cin >> startX >> startY;


    board[startX][startY] = 0;

    if (knight(startX, startY, 1)) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++)
                cout << setw(4) << board[i][j];
            cout << endl;
        }
    } else {
        cout << "No solution.";
    }

    return 0;
}