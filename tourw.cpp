#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>


using namespace std;

const int n = 8;

int board[n][n];
int degree[n][n];

int dir[8][2] = {{2, 1}, {1, 2}, {-1, 2}, {-2, 1}, {-2, -1}, {-1, -2}, {1, -2}, {2, -1} };


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


void updateDegree(int x, int y, int delta) {
    for (int d = 0; d < 8; d++) {
        int nx = x + dir[d][0];
        int ny = y + dir[d][1];
        if (nx >= 0 && nx < n && ny >= 0 && ny < n && board[nx][ny] == -1) {
            degree[nx][ny] += delta;
        }
    }
}



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
    if (step == n * n + 1)
        return true;

    int moveDirs[8];
    int moveDegrees[8];
    int moveCount = getMoves(x, y, moveDirs, moveDegrees);

    if (moveCount == 0)
        return false;

    int minDegree = moveDegrees[0];
    for (int i = 1; i < moveCount; i++)
        if (moveDegrees[i] < minDegree)
            minDegree = moveDegrees[i];


    int candidates[8];
    int candCount = 0;
    for (int i = 0; i < moveCount; i++) {
        if (moveDegrees[i] == minDegree) {
            candidates[candCount++] = i;
        }
    }


    for (int i = 0; i < candCount - 1; i++) {
        int r = i + rand() % (candCount - i);
        int temp = candidates[i];
        candidates[i] = candidates[r];
        candidates[r] = temp;
    }


    for (int i = 0; i < candCount; i++) {
        int dirIdx = moveDirs[candidates[i]];
        int nx = x + dir[dirIdx][0];
        int ny = y + dir[dirIdx][1];

        board[nx][ny] = step;
        updateDegree(nx, ny, -1);

        if (knightTour(nx, ny, step + 1))
            return true;


        updateDegree(nx, ny, +1);
        board[nx][ny] = -1;
    }

    return false;
}

int main() {
    srand(time(0));
    int startx, starty;
    cout << "Input starting position (0-"<< n-1 << ") (0-"<< n-1 << "): ";
   
    cin >> startx >> starty;

    
    if (startx < 0 || startx >= n || starty < 0 || starty >= n) {
        cout << "0-7 only";
        return 0;
    }
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            board[i][j] = -1;
    initDegree();

    board[startx][starty] = 1;
    updateDegree(startx, starty, -1);

    if (knightTour(startx, starty, 2)) {
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
