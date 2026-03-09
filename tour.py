# Python program to for Knight's tour problem using
# Warnsdorff's algorithm
import random
import time

start = None


class Cell:
    def __init__(self, x, y):
        self.x = x
        self.y = y

N = 8


cx = [1, 1, 2, 2, -1, -1, -2, -2]
cy = [2, -2, 1, -1, 2, -2, 1, -1]


def limits(x, y):
    return ((x >= 0 and y >= 0) and (x < N and y < N))


def isempty(a, x, y):
    return (limits(x, y)) and (a[y * N + x] < 0)


def getDegree(a, x, y):
    count = 0
    for i in range(N):
        if isempty(a, (x + cx[i]), (y + cy[i])):
            count += 1
    return count


def nextMove(a, cell):
    min_deg_idx = -1
    c = 0
    min_deg = (N + 1)
    nx = 0
    ny = 0


    start = random.randint(0, 1000) % N
    for count in range(0, N):
        i = (start + count) % N
        nx = cell.x + cx[i]
        ny = cell.y + cy[i]
        c = getDegree(a, nx, ny)
        if ((isempty(a, nx, ny)) and c < min_deg):
            min_deg_idx = i
            min_deg = c


    if (min_deg_idx == -1):
        return None


    nx = cell.x + cx[min_deg_idx]
    ny = cell.y + cy[min_deg_idx]


    a[ny * N + nx] = a[(cell.y) * N + (cell.x)] + 1


    cell.x = nx
    cell.y = ny

    return cell


def printA(a):
    for i in range(N):
        for j in range(N):
            print("%d\t" % a[j * N + i], end="")
        print()


def neighbour(x, y, xx, yy):
    for i in range(N):
        if ((x + cx[i]) == xx) and ((y + cy[i]) == yy):
            return True
    return False


def findClosedTour(sx, sy):

    a = [-1] * N * N


    cell = Cell(sx, sy)

    a[cell.y * N + cell.x] = 1


    ret = None
    for i in range(N * N - 1):
        ret = nextMove(a, cell)
        if ret == None:
            return False


    if not neighbour(ret.x, ret.y, sx, sy):
        return False
    printA(a)
    return True


# Driver Code
if __name__ == '__main__':
    # While we don't get a solution
    sx = int(input("X: "))
    sy = int(input("Y: "))
    start = time.time()
    while not findClosedTour(sx, sy):
        pass
    end = time.time()
    print(f"{round(end-start, 8)} seconds")