#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <limits.h>
#include <tgmath.h>
#include <chrono>

using namespace std;

const int DEFALT_START_POSITION = -1;
const int FLAG = -1;

vector<vector<int>> puzzle;
vector<string> stepsNeeded;
int countOfRowsAndCols = 0; // Matrix is n*n so number of rows is equal to number of columns
int finalPositionOfZero = 0;
int curPositionOfZero = 0;

int manhattanAlgorithm()
{
    int result = 0;
    int positionsRemainingToZero = finalPositionOfZero;
    for (int i = 0; i < countOfRowsAndCols; i++)
    {
        for (int j = 0; j < countOfRowsAndCols; j++)
        {
            int curCell = puzzle[i][j];
            if (curCell) // if current cell is not null
            {
                if (positionsRemainingToZero > 0)
                {
                    positionsRemainingToZero--;
                    result += abs((curCell - 1) / countOfRowsAndCols - i) + abs((curCell - 1) % countOfRowsAndCols - j);
                }
                else
                {
                    result += abs(curCell / countOfRowsAndCols - i) + abs(curCell % countOfRowsAndCols - j);
                }
            }
        }
    }
    return result;
}

void swapElements(int positionOftA, int positionOfB)
{
    // we need this because its a 2d array
    int i = positionOftA / countOfRowsAndCols; // rols
    int j = positionOftA % countOfRowsAndCols; // cols

    int i2 = positionOfB / countOfRowsAndCols;
    int j2 = positionOfB % countOfRowsAndCols;

    swap(puzzle[i][j], puzzle[i2][j2]);
}

// 0 is in the most far left column
bool canMoveRight()
{
    if (curPositionOfZero % countOfRowsAndCols == 0)
    {
        return false;
    }
    swapElements(curPositionOfZero, curPositionOfZero - 1);
    curPositionOfZero--;
    return true;
}

// 0 is in the most far right column
bool canMoveLeft()
{
    if (curPositionOfZero % countOfRowsAndCols == countOfRowsAndCols - 1)
    {
        return false;
    }
    swapElements(curPositionOfZero, curPositionOfZero + 1);
    curPositionOfZero++;
    return true;
}

// 0 is in the last row
bool canMoveUp()
{
    if (curPositionOfZero / countOfRowsAndCols == countOfRowsAndCols - 1)
    {
        return false;
    }
    swapElements(curPositionOfZero, curPositionOfZero + countOfRowsAndCols);
    curPositionOfZero += countOfRowsAndCols;
    return true;
}

// 0 is in the rirst row
bool canMoveDown()
{
    if (curPositionOfZero / countOfRowsAndCols == 0)
    {
        return false;
    }
    swapElements(curPositionOfZero, curPositionOfZero - countOfRowsAndCols);
    curPositionOfZero -= countOfRowsAndCols;
    return true;
}

int posOfZero()
{
    for (int i = 0; i < countOfRowsAndCols; i++)
    {
        for (int j = 0; j < countOfRowsAndCols; j++)
        {
            if (puzzle[i][j] == 0) // the empty cell
            {
                return countOfRowsAndCols * i + j;
            }
        }
    }
}

int findFunc(int funcG, int threshold)
{
    // f(m) = g(n) + h(n) where g(n) is the cost to travel from the root to node n
    // and h(n) is the problem specific heuristic estimate of the cost to travel from n to the goal
    int funcF = funcG + manhattanAlgorithm();
    int min = INT_MAX;

    if (threshold < funcF)
    {
        return funcF;
    }

    if (manhattanAlgorithm() == 0)
    {
        // print int in new line
        cout << stepsNeeded.size() - 1 << endl;
        for (int i = 1; i < stepsNeeded.size(); i++)
        {
            cout << stepsNeeded[i] << "\n";
        }
        return FLAG;
    }

    if (stepsNeeded.back() != "up" && canMoveDown())
    {
        stepsNeeded.push_back("down");
        int curElem = findFunc(funcG + 1, threshold); // explore the new state
        if (curElem == FLAG)
        {
            return FLAG;
        }
        if (curElem < min)
        {
            min = curElem;
        }
        stepsNeeded.pop_back(); // removes the "up" move from the stepsNeeded vector. This is done because the algorithm is backtracking to explore a different path
        canMoveUp();
    }

    if (stepsNeeded.back() != "down" && canMoveUp())
    {
        stepsNeeded.push_back("up");
        int curElem = findFunc(funcG + 1, threshold);
        if (curElem == FLAG)
        {
            return FLAG;
        }
        if (curElem < min)
        {
            min = curElem;
        }
        stepsNeeded.pop_back();
        canMoveDown();
    }

    if (stepsNeeded.back() != "left" && canMoveRight())
    {
        stepsNeeded.push_back("right");
        int curElem = findFunc(funcG + 1, threshold);
        if (curElem == FLAG)
        {
            return FLAG;
        }
        if (curElem < min)
        {
            min = curElem;
        }
        stepsNeeded.pop_back();
        canMoveLeft();
    }

    if (stepsNeeded.back() != "right" && canMoveLeft())
    {
        stepsNeeded.push_back("left");
        int curElem = findFunc(funcG + 1, threshold);
        if (curElem == FLAG)
        {
            return FLAG;
        }
        if (curElem < min)
        {
            min = curElem;
        }
        stepsNeeded.pop_back();
        canMoveRight();
    }

    return min;
}

void iterativeDeepeningAStar()
{
    int threshold = manhattanAlgorithm();
    stepsNeeded.push_back("init");

    while (true)
    {
        int curElem = findFunc(0, threshold);
        if (curElem == FLAG)
        {
            break;
        }
        threshold = curElem;
    }
}

void inputPuzzle()
{
    vector<int> row(countOfRowsAndCols);
    for (int i = 0; i < countOfRowsAndCols; i++)
    {
        for (int j = 0; j < countOfRowsAndCols; j++)
        {
            cin >> row[j];
        }
        puzzle.push_back(row); // Adds the row vector to the puzzle vector. The puzzle vector is going to two-dimensional vector that represents the entire puzzle
    }
}

void solvePuzzle()
{
    int puzzleSize, goalPositionZero;
    cin >> puzzleSize >> goalPositionZero;

    puzzleSize++; // 8 -> 9, 15 -> 16
    countOfRowsAndCols = sqrt(puzzleSize);

    inputPuzzle();

    // Timer
    // auto start = chrono::high_resolution_clock::now();
    //*********

    finalPositionOfZero = goalPositionZero;
    if (goalPositionZero == -1) // default start position is -1
    {
        finalPositionOfZero = puzzleSize - 1;
    }

    curPositionOfZero = posOfZero();

    iterativeDeepeningAStar();

    // Timer
    // auto end = chrono::high_resolution_clock::now();
    // chrono::duration<double> elapsed = end - start;
    // cout << elapsed.count();
}

int main()
{
    solvePuzzle();
    return 0;
}

/*
Input:
 8
-1
1 2 3
4 5 6
0 7 8

Output:
2
left
left

Input:
8
8
1 2 3
4 5 6
0 7 8

Output:
2
left
left

Input:
8
4
1 2 3
4 5 0
6 7 8

Output:
1
right

Input:
8
0
1 2 0
3 4 5
6 7 8
Output:
2
right
right


Input:
8
-1
1 2 3
4 5 6
7 8 0

Output: 0


Input:
8
-1
1 2 3
4 5 6
8 7 0

Output: -1


Input:
15
-1
1 2 3 4
5 6 7 8
9 10 11 12
13 14 0 15
Output:
1
left

Input:
8
-1
6 5 3
2 4 8
7 0 1

Output:
21
left
down
down
right
right
up
left
up
right
down
left
down
left
up
right
down
right
up
up
left
left


15
-1
10 5 8 11
2 12 9 6
13 1 0 15
4 7 3 14
********
50
down
right
up
left
up
right
right
down
left
up
left
left
down
down
right
up
left
down
down
right
right
right
up
left
up
up
left
left
down
down
right
right
down
left
up
up
right
down
down
right
up
left
left
left
down
right
up
up
left
up
*/
