#include <iostream>
#include <vector>
#include <string>
#include<limits.h>

using namespace std;

const int ELEMENT_TO_COMPARE = 1;//Always compare the middle element of the row/column/diagonal/secondary diagonal

int maximizer(int alpha, int beta, int depth);
int minimizer(int alpha, int beta, int depth);

bool isHumanTurn = false;
char computerSymbol = ' ', humanSymbol = ' ';
int maxScore = 10, minScore = -10, invalidPosition = -1;
char emptyCell = '-';

//here we create the board which is empty at the beginning 3x3
vector<string> gameGrid =
{
    { "---"},
    { "---"},
    { "---"}
};

//function checks for a winner or a draw and assigns scores accordingly
//parameter is used to penalize winning moves that take longer to achieve.
int evaluateBoard(int depth)
{
    //check if all elements in the row are the same
    for (int row = 0; row < 3; row++)
    {
        if (gameGrid[row][0] == gameGrid[row][1] && gameGrid[row][1] == gameGrid[row][2])//check if all elements in the row are the same
        {
            if (gameGrid[row][ELEMENT_TO_COMPARE] == computerSymbol)//if the winner is the computer
            {
                return maxScore - depth;
            }
            else if (gameGrid[row][ELEMENT_TO_COMPARE] == humanSymbol)//if the winner is the person
            {
                return minScore + depth;
            }
        }
    }

    //check if all elements in the column are the same
    for (int col = 0; col < 3; col++)
    {
        if (gameGrid[0][col] == gameGrid[1][col] && gameGrid[1][col] == gameGrid[2][col])
        {
            if (gameGrid[ELEMENT_TO_COMPARE][col] == computerSymbol)
            {
                return maxScore - depth;
            }
            else if (gameGrid[ELEMENT_TO_COMPARE][col] == humanSymbol)
            {
                return minScore + depth;
            }
        }
    }

    //check if all elements in the main diagonal are the same
    if (gameGrid[0][0] == gameGrid[1][1] && gameGrid[1][1] == gameGrid[2][2])
    {
        if (gameGrid[ELEMENT_TO_COMPARE][ELEMENT_TO_COMPARE] == computerSymbol)
        {
            return maxScore - depth;//if the winner is the computer
        }
        else if (gameGrid[ELEMENT_TO_COMPARE][ELEMENT_TO_COMPARE] == humanSymbol)
        {
            return minScore + depth;//if the winner is the person
        }
    }

    //check if all elements in the secondary diagonal are the same
    if (gameGrid[0][2] == gameGrid[1][1] && gameGrid[1][1] == gameGrid[2][0])
    {
        if (gameGrid[ELEMENT_TO_COMPARE][ELEMENT_TO_COMPARE] == computerSymbol)
        {
            return maxScore - depth;
        }
        else if (gameGrid[ELEMENT_TO_COMPARE][ELEMENT_TO_COMPARE] == humanSymbol)
        {
            return minScore + depth;
        }
    }

    return 0;
}

void printBoard() {

    printf("-------------\n");

    for (int i = 0; i < 3; i++) {
        printf("| ");
        for (int j = 0; j < 3; j++) {
            printf("%c", gameGrid[i][j]);//%c is used to print a character
            printf(" | ");
        }
        printf("\n-------------\n");
    }
}

//check if there are empty cells
bool areThereMoreMovesLeft()
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (gameGrid[i][j] == emptyCell)
            {
                return true;
            }
        }
    }
    return false;
}


int maximizer(int alpha, int beta, int depth)
{
    int curScore = evaluateBoard(depth);

    //if the current score is not 0, then it means that there is a winner
    if (curScore != 0)
    {
        return curScore;
    }

    //if there are no more moves left, then it is a draw
    if (areThereMoreMovesLeft() == false) //same as
    {
        return 0;
    }
    //the best score is the lowest possible score
    int bestScore = INT_MIN;

    //what these cycles do is that they check all the possible moves that the person can make
    //The function then iterates over all cells of the game board. For each empty cell, it simulates the computer making a move there,
    //then calls the minimizer function to simulate the human's response.
    //The score returned by the minimizer function is compared with bestScore, and if it's higher, bestScore is updated.    for (int i = 0; i < 3; i++)
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; j++)
        {
            if (gameGrid[i][j] == emptyCell)
            {
                //make the move
                gameGrid[i][j] = computerSymbol;

                //simulate the human's response and update if needed
                bestScore = max(bestScore, minimizer(alpha, beta, depth + 1));

                gameGrid[i][j] = emptyCell;

                //if bestScore is greater than or equal to beta, the function returns bestScore immediately,
                // as it has found a score that is at least as good as the best score the human can achieve, so there's no need to explore this branch further.
                if (bestScore >= beta)
                {
                    return bestScore;
                }
                alpha = max(alpha, bestScore);
            }
        }
    }
    //if none of the above return statements are triggered, the function returns bestScore, which is the best score that can be achieved from the current state, assuming optimal play from both players.
    return bestScore;
}

int minimizer(int alpha, int beta, int depth)
{
    int curScore = evaluateBoard(depth);
    //if the current score is not 0, then it means that there is a winner
    if (curScore != 0)
    {
        return curScore;
    }

    //if there are no more moves left, then it is a draw
    if (!areThereMoreMovesLeft())
    {
        return 0;
    }

    int bestScore = INT_MAX;


    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (gameGrid[i][j] == emptyCell)
            {
                gameGrid[i][j] = humanSymbol;

                bestScore = min(bestScore, maximizer(alpha, beta, depth + 1));

                gameGrid[i][j] = emptyCell;

                if (bestScore <= alpha)
                {
                    return bestScore;
                }
                beta = min(beta, bestScore);
            }
        }
    }
    return bestScore;
}

//this function finds the best turn for the computer
pair<int, int> findBestTurnForComputer()
{
    int bestValue = INT_MIN;
    pair<int, int> bestNextTurn;

    bestNextTurn.first = invalidPosition;
    bestNextTurn.second = invalidPosition;

    //The function then iterates over all cells of the game board. For each empty cell, it simulates the computer making a move there,
    // then calls the minimizer function to simulate the human's response. The score returned by the minimizer function is compared with
    // bestValue, and if it's higher, bestValue is updated and bestNextTurn is set to the current cell
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (gameGrid[i][j] == emptyCell)
            {
                //simulate the computer's move
                gameGrid[i][j] = computerSymbol;

                //simulate the human's response
                int curTurnValue = minimizer(INT_MIN, INT_MAX, 0);

                //undo the move of the computer
                gameGrid[i][j] = emptyCell;

                //if the current turn is better than the best turn, then update the best turn
                if (curTurnValue > bestValue)
                {
                    bestNextTurn.first = i;
                    bestNextTurn.second = j;
                    bestValue = curTurnValue;
                }
            }
        }
    }
    return bestNextTurn;
}

//If the returned score is not zero, it means that a terminal state has been reached (someone has won or the game is a draw), so it returns true
bool isThereWinner()
{
    if (evaluateBoard(0) != 0)
    {
        return true;
    }
    return false;
}

//The makeTurn function is used to make a move on the game board.
bool makeTurn(int i, int j, bool isHumanTurn)
{
    if (gameGrid[i][j] == emptyCell)
    {
        if (isHumanTurn == true)
        {
            gameGrid[i][j] = humanSymbol;
        }
        else
        {
            gameGrid[i][j] = computerSymbol;
        }


        printf("=============\n");

        printBoard();

        if (isThereWinner() == true)
        {
            if (isHumanTurn == true)
            {
                printf("Congratulations, you are the winner!\n");
            }
            else
            {
                printf("Sorry, you have lost!\n");
            }
        }

        return true;
    }

    printf("This cell is not empty! Please choose another cell!\n");
    return false;
}

void startGame() {
    printBoard();

    printf("Do you want to be the first to start the game? 0/1 : ");
    cin >> isHumanTurn;

    if (isHumanTurn == 1)
    {
        computerSymbol = 'o';
        humanSymbol = 'x';
    }
    else
    {
        computerSymbol = 'x';
        humanSymbol = 'o';
    }

    while (areThereMoreMovesLeft() && !isThereWinner())
    {
        int i, j;
        if (isHumanTurn == true)
        {
            do
            {
                printf("Input Row:");
                cin >> i;
                printf("Input Column:");
                cin>> j;

            } while (makeTurn(i - 1, j - 1, isHumanTurn) == false);//this means that the person has to input the row and column again
            isHumanTurn = !isHumanTurn;//if true, then it becomes false and if false, then it becomes true
            continue;
        }
        pair<int, int> bestTurn = findBestTurnForComputer();
        makeTurn(bestTurn.first, bestTurn.second, isHumanTurn);
        isHumanTurn = !isHumanTurn;
    }

}

int main()
{
    startGame();
    return 0;
}