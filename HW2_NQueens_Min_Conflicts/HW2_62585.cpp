#include <iostream>
#include <vector>
#include <string>
#include <time.h>
#include <chrono>
#include <iomanip>
using namespace std;

bool CONFLICTS_EXIST = true;
int boardSize = 0; // board size equals to the number of queens and the size of the board from each side

// Return the column of the queen with the most conflicts
// Break connections randomly

// Queens is a vector that contains the position of each queen in the column with the same index
// For example if queens[0] = 2 this means that the queen in the column 0 is in the row 2 so there is queen on cordinates (0,2)

// the vector queensInRow contains the number of queens in each row
// the vector d1 contains the number of queens in each primamary diagonal(/)
// the vector d2 contains the number of queens in each secondary diagonal(\)

// The function returns the column with the maximum conflicts, considering the current queen positions.
int findMaxConflictsCol(vector<int> &queens, vector<int> &queensInRow, vector<int> &d1, vector<int> &d2)
{
    // This variable is used to keep track of the maximum number of conflicts found
    int maxConf = -1;
    // This vector is used to store the columns with the maximum number of conflicts
    vector<int> colsWithMaxConf;

    // Initialize the variables to store the current row and the current count of conflicts
    int curRow, curConf;

    // Loop through each column
    for (int curCol = 0; curCol < boardSize; curCol++)
    {
        // Get the row of the queen in the current column
        curRow = queens[curCol];
        // the number of conflicts is the number of queens in the same row + the number of queens in the same diagonal /
        // + the number of queens in the same diagonal \
        // we subtract 3 because we have added 3 in the putQueens function because we have put 3 queens in the same row, column and diagonal
        curConf = queensInRow[curRow] + d1[curCol - curRow + boardSize - 1] + d2[curCol + curRow] - 3;

        // when we have more than one column with the same number of conflicts
        if (curConf == maxConf)
        {
            // Add the current column to the vector of columns with the maximum number of conflicts
            colsWithMaxConf.push_back(curCol);
        }
        // when we have more than one column with the same number of conflicts we need to, then we choose randomly so we don't always choose the same column and end up in endless loop
        else if (curConf > maxConf)
        {
            maxConf = curConf;                 // set the current number of conflicts to be the new maximum number of conflicts
            colsWithMaxConf.clear();           // clear the vector of columns with the maximum number of conflicts
            colsWithMaxConf.push_back(curCol); // add the current column to the vector of columns with the maximum number of conflicts
        }
    }

    // If the maximum number of conflicts is 0, then there are no conflicts
    if (maxConf == 0)
    {
        CONFLICTS_EXIST = false; // seve that there are no conflicts
    }

    // Generate a random index between 0 and the size of the vector of columns with the maximum number of conflicts
    // and return the randomly selected column with the maximum number of conflicts
    // using random index is important because if we have more than one column with the same number of conflicts
    // we need to choose randomly so we don't always choose the same column end up in endless loop
    int randIndex = rand() % colsWithMaxConf.size();

    // Return the randomly selected column with the maximum number of conflicts
    return colsWithMaxConf[randIndex];
}

// this function returns the row with the minimum number of conflicts
int findMinConflictsRow(vector<int> &queens, vector<int> &queensInRow, vector<int> &d1, vector<int> &d2, int col)
{
    // Initialize the value of minConf to a value larger than the maximum possible value
    int minConf = boardSize + 1;
    // Initialize the vector of rows with the minimum number of conflicts
    vector<int> rowsWithMinConf;

    // Initialize the variable to store the current number of conflicts
    int curConf;

    // Loop through each row
    for (int curRow = 0; curRow < boardSize; curRow++)
    {
        if (queens[col] == curRow) // wht we are doing here is that we are checking if there is a queen in the same row as the current row
        {
            curConf = queensInRow[curRow] + d1[col - curRow + boardSize - 1] + d2[col + curRow] - 3; // why we subtract 3 is because we have added 3 in the putQueens function because we have put 3 queens in the same row, column and diagonal
            // this formula is the same as the one in the findMaxConflictsCol function   curConf = queensInRow[curRow] + d1[col - curRow + boardSize - 1] + d2[col + curRow] - 3;
        }
        // Otherwise, just use the current number of conflicts
        else
        {
            curConf = queensInRow[curRow] + d1[col - curRow + boardSize - 1] + d2[col + curRow]; // we didn't subtract 3 here because there is no queen in the same row as the current row
        }

        // If the current number of conflicts is the same as the current minimum number of conflicts
        if (curConf == minConf)
        {
            // Add the current row to the vector of rows with the minimum number of conflicts
            rowsWithMinConf.push_back(curRow);
        }
        // If the current number of conflicts is less than the current minimum number of conflicts
        else if (curConf < minConf)
        {
            // Set the current number of conflicts to be the new minimum number of conflicts
            minConf = curConf;
            // Clear the vector of rows with the minimum number of conflicts because we have found a new minimum number of conflicts
            rowsWithMinConf.clear();
            // Add the current row to the vector of rows with the minimum number of conflicts
            rowsWithMinConf.push_back(curRow);
        }
    }

    // Generate a random index between 0 and the size of the vector of rows with the minimum number of conflicts
    int randIndex = rand() % rowsWithMinConf.size();

    // Return the randomly selected row with the minimum number of conflicts
    return rowsWithMinConf[randIndex];
}

// This function initializes the queen positions on the chessboard, marking rows and diagonals as occupied
// It ensures that queens are placed such that they do not conflict in the same row, column, or diagonals
// For example if we have 4 queens we put the first queen in the second column, the second queen in the fourth column, the third queen in the first column and the fourth queen in the third column
void putQueens(vector<int> &queens, vector<int> &queensInRow, vector<int> &d1, vector<int> &d2)
{
    // Initialize the column to 1 because we want to put the first queen in the second column
    int col = 1;

    for (int row = 0; row < boardSize; row++)
    {
        // Put the queen in the current column and row
        queens[col] = row;
        // Mark that the row is taken
        queensInRow[row]++;
        // Mark that the primry(/) diagonal is taken
        d1[col - row + boardSize - 1]++;

        // mark that the diagonal secondary(\) is taken
        d2[col + row]++;

        // move over two columns because this way we ensure that we don't have queens in the same row, column and diagonal
        col += 2;
        // if we have reached the end of the board we go back to the first column
        if (col >= boardSize)
        {
            col = 0;
        }
    }
}

// This function updates the state after moving a queen to a new position
//  It adjusts the counts of queens in rows and diagonals based on the new queen position.
void updateState(vector<int> &queens, vector<int> &queensInRow, vector<int> &d1, vector<int> &d2, int row, int col)
{
    int prevRow = queens[col];           // Here we get the previous row of the queen in the current column
    queensInRow[prevRow]--;              // Now we decrease the number of queens in the same row because we are moving the queen to a new position
    d1[col - prevRow + boardSize - 1]--; // Now we decrease the number of queens in the primary diagonal(/) because we are moving the queen to a new position
    d2[col + prevRow]--;                 // Now we decrease the number of queens in the secondary diagonal(\)

    queens[col] = row;               // Here we move the queen to the new position
    queensInRow[row]++;              // We increase the number of queens in the same row because we have moved the queen to a new position
    d1[col - row + boardSize - 1]++; // Now we increase the number of queens in the primary diagonal(/) because we have moved the queen to a new position
    d2[col + row]++;                 // Now we increase the number of queens in the secondary diagonal(\)
}

// This function prints the chessboard configuration with '*' representing queens and '_' representing empty squares
// It is used for visualization.
void printQueens(vector<int> &queens)
{
    for (int i = 0; i < boardSize; i++)
    {
        for (int j = 0; j < boardSize; j++)
        {
            if (queens[j] == i) // if we have a queen in the current row and column we print *
            {
                printf("* ");
            }
            else // if we don't have a queen in the current row and column we print _
            {
                printf("_ ");
            }
        }
        cout << endl;
    }
}

// This function attempts to find solution the N-Queens problem using a local search algorithm
// It repeatedly moves queens to reduce conflicts, with a maximum number of iterations to escape local minima
void findSolution(vector<int> &queens, vector<int> &queensInRow, vector<int> &d1, vector<int> &d2)
{
    int iter = 0; // this is the number of iterations and we use it to escape from local minimum
    int k = 1;    // if we encounter a lical minimum we give it a chance to escape from the local minimum and find a better solution.
    int col, row;

    // Loop until we have a solution or we reach the maximum number of iterations
    while (iter <= k * boardSize) // k*boardSize is the maximum number of iterations because we have boardSize queens and each queen can be moved boardSize times
    {
        // Find the column with the maximum number of conflicts
        col = findMaxConflictsCol(queens, queensInRow, d1, d2);

        if (CONFLICTS_EXIST == false) // this means that hasConflicts is false and we have a solution
        {
            break;
        }

        // Find the row with the minimum number of conflicts
        row = findMinConflictsRow(queens, queensInRow, d1, d2, col);

        // Move the queen from the column with the maximum number of conflicts
        // to the row with the minimum number of conflicts
        updateState(queens, queensInRow, d1, d2, row, col);

        iter++; // a new iteration hs been completed
    }

    // If we have not found a solution, restart the search
    if (CONFLICTS_EXIST == true) // if conflicts_exist is true we restart the search until we find a solution
    {
        findSolution(queens, queensInRow, d1, d2);
    }
}

void startTask()
{
    cin >> boardSize;
    vector<int> queens(boardSize), r(boardSize), d1(2 * boardSize), d2(2 * boardSize);

    putQueens(queens, r, d1, d2);
    findSolution(queens, r, d1, d2);

    //if the result is not found print -1
    if (CONFLICTS_EXIST == true)
    {
        cout << -1 << endl;
    }
    else if (boardSize < 100)
    {
        printQueens(queens);
    }
    else
    {
        cout << fixed << setprecision(3) << (double)clock() / CLOCKS_PER_SEC << endl;
    }


}

int main()
{
    startTask();
    return 0;
}
