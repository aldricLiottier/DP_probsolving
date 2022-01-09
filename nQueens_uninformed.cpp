#include <stdbool.h>
#include <stdio.h>

#include <string>
#include <iostream>
#include <vector>
#define ERROR 84

int ld[30] = { 0 };
int rd[30] = { 0 };
int cl[30] = { 0 };

typedef std::vector<std::vector<int>> Board;

// Prints the given board
void printBoard(Board board)
{
    int size = board.size();

    for (int i = 0; i < size; ++i) {
        for (auto it = begin(board[i]); it != end(board[i]); it++)
            std::cout << *it << " ";
        std::cout << std::endl;
    }
}

bool solveNQueens(Board *board, int col)
{
    int size = board->size();

    if (col >= size)
        return true;
    
    for (int i = 0; i < size; i++) {
        if ((ld[i - col + size - 1] != 1 &&
             rd[i + col] != 1) && cl[i] != 1) {
            (*board)[i][col] = 1;
            ld[i - col + size - 1] =
            rd[i + col] = cl[i] = 1;

            // returns true if the next step is true asw
            if (solveNQueens(board, col + 1)) {
                return true;
            }

            // not a solution state
            // resets the current cell before going back with the recursion
            (*board)[i][col] = 0;
            ld[i - col + size - 1] =
            rd[i + col] = cl[i] = 0;
        }
    }
    return false;
}

int main(int ac, char **av)
{
    int size = 0;
    if (ac != 2)
        return ERROR;

    // check if the argument is valid
    std::string s_arg(av[1]);
    try {
        size = std::stoi(s_arg, nullptr);
    } catch (std::exception e) {
        std::cerr << "Error" << std::endl;
        return 84;
    }
    
    if (size == 2 || size == 3) {
        std::cerr << "Invalid size" << std::endl;
        return ERROR;
    }
    
    // create an empty board
    std::vector<int> line(size, 0);
    Board board(size, line);

    // start algorithm
    solveNQueens(&board, 0);
    
    //display the result with queens on the board
    printBoard(board);

    return 0;
}
