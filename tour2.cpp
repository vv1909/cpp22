#include <iostream>
#include <algorithm>
#include <iomanip>
using namespace std;




int dir[8][2] = { {2, 1}, {1, 2}, {-1, 2}, {-2, 1}, {-2, -1}, {-1, -2}, {1, -2}, {2, -1}};



struct Move {
    int options;
    int dirIndex;
};


int countMoves(int board[8][8], int n, int x, int y) {
    int count = 0;

    for (int i = 0; i < 8; i++) {
        int nx = x + dir[i][0];
        int ny = y + dir[i][1];

        if (nx >= 0 && ny >= 0 && nx < n && ny < n && board[nx][ny] == -1) {
            count++;
        }
    }

    return count;
}


int getSortedMoves(int board[8][8], int n, int x, int y, Move moves[8]) {
    int moveCount = 0;

    for (int i = 0; i < 8; i++) {
        int nx = x + dir[i][0];
        int ny = y + dir[i][1];

        if (nx >= 0 && ny >= 0 && nx < n && ny < n &&
            board[nx][ny] == -1) {

            moves[moveCount].options = countMoves(board, n, nx, ny);
            moves[moveCount].dirIndex = i;
            moveCount++;
        }
    }

    sort(moves, moves + moveCount, [](Move a, Move b) {
        return a.options < b.options;
    });

    return moveCount;
}

bool knightTour(int board[8][8], int n, int x, int y, int step) {
    if (step == n * n)
        return true;

    Move moves[8];
    int moveCount = getSortedMoves(board, n, x, y, moves);

    for (int i = 0; i < moveCount; i++) {
        int dirIdx = moves[i].dirIndex;
        int nx = x + dir[dirIdx][0];
        int ny = y + dir[dirIdx][1];

        board[nx][ny] = step;

        if (knightTour(board, n, nx, ny, step + 1))
            return true;

        board[nx][ny] = -1;
    }

    return false;
}



int main() {
    int n = 8;
    int startx = 0;
    int starty = 0;

    cout << "Input coordinates (0-7) (0-7): ";
    cin >> startx >> starty;


    int board[8][8];


    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            board[i][j] = -1;

    board[startx][starty] = 0;

    if (knightTour(board, n, startx, starty, 1) == true) {

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                cout << setw(4) << board[i][j];
            }
            cout << endl;
        }
    }
    else
        {
            cout << "no solution";
        }
    
    return 0;
}