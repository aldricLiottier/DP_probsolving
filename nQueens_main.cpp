#include <stdbool.h>
#include <stdio.h>

#include <string>
#include <iostream>
#include <vector>
#define ERROR 84

/* ld is an array where its indices indicate row-col+N-1
(N-1) is for shifting the difference to store negative
indices */
int ld[30] = { 0 };
/* rd is an array where its indices indicate row+col
and used to check whether a queen can be placed on
right diagonal or not*/
int rd[30] = { 0 };
/*column array where its indices indicates column and
used to check whether a queen can be placed in that
	row or not*/
int cl[30] = { 0 };
/* A utility function to print solution */

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

///* A recursive utility function to solve N
//Queen problem */
//// bool solveNQUtil(int board[N][N], int col);
//bool solveNQUtil(int **board, int size, int col)
//{
//    /* base case: If all queens are placed
//    then return true */
//    if (col >= size)
//        return true;
//
//    /* Consider this column and try placing
//    this queen in all rows one by one */
//    for (int i = 0; i < size; i++) {
//        /* Check if the queen can be placed on
//        board[i][col] */
//        /* A check if a queen can be placed on
//        board[row][col].We just need to check
//        ld[row-col+n-1] and rd[row+coln] where
//        ld and rd are for left and right
//        diagonal respectively*/
//        if ((ld[i - col + size - 1] != 1 &&
//             rd[i + col] != 1) && cl[i] != 1) {
//            /* Place this queen in board[i][col] */
//            board[i][col] = 1;
//            ld[i - col + size - 1] =
//            rd[i + col] = cl[i] = 1;
//
//            /* recur to place rest of the queens */
//            if (solveNQUtil(board, size, col + 1))
//                return true;
//
//            /* If placing queen in board[i][col]
//            doesn't lead to a solution, then
//            remove queen from board[i][col] */
//            board[i][col] = 0; // BACKTRACK
//            ld[i - col + size - 1] =
//            rd[i + col] = cl[i] = 0;
//        }
//    }
//
//    /* If the queen cannot be placed in any row in
//        this colum col then return false */
//    return false;
//}

/* This function solves the N Queen problem using
Backtracking. It mainly uses solveNQUtil() to
solve the problem. It returns false if queens
cannot be placed, otherwise, return true and
prints placement of queens in the form of 1s.
Please note that there may be more than one
solutions, this function prints one of the
feasible solutions.*/
// bool solveNQ()

bool solveNQueens(Board *board, int col)
{
    int size = board->size();

    if (col >= size)
        return true;

    /* Consider this column and try placing
    this queen in all rows one by one */
    for (int i = 0; i < size; i++) {
        /* Check if the queen can be placed on
        board[i][col] */
        /* A check if a queen can be placed on
        board[row][col].We just need to check
        ld[row-col+n-1] and rd[row+coln] where
        ld and rd are for left and right
        diagonal respectively*/
        if ((ld[i - col + size - 1] != 1 &&
             rd[i + col] != 1) && cl[i] != 1) {
            /* Place this queen in board[i][col] */
            (*board)[i][col] = 1;
            ld[i - col + size - 1] =
            rd[i + col] = cl[i] = 1;

            /* recur to place rest of the queens */
            if (solveNQueens(board, col + 1)) {
                return true;
            }
            /* If placing queen in board[i][col]
            doesn't lead to a solution, then
            remove queen from board[i][col] */
            (*board)[i][col] = 0; // BACKTRACK
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
