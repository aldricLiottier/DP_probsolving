//
// Created by Utilisateur on 10/01/2022.
//

#include "NQueens.hpp"
#include <iostream>

#define ARGUMENT_ERROR -1
#define ERROR 84

void printGrid(const Board &board)
{
    int size = board.size();

    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            if (board[y][x] == 1)
                std::cout << "Q ";
            else
                std::cout << "_ ";
        }
        std::cout << std::endl;
    }
}

int check_args(int ac, char **av)
{
    int size = 0;
    if (ac != 2)
        return ARGUMENT_ERROR;

    // check if the argument is valid
    std::string s_arg(av[1]);
    try {
        size = std::stoi(s_arg, nullptr);
    } catch (std::exception e) {
        std::cerr << "Error" << std::endl;
        return ARGUMENT_ERROR;
    }

    return size;
}

int main(int ac, char **av)
{
    int size = check_args(ac, av);

    if (size == ARGUMENT_ERROR)
        return ERROR;
    else if (size == 2 || size == 3) {
        std::cerr << "Invalid size" << std::endl;
        return ERROR;
    }

    NQueens algo(size);

    // algo.Compute(NQueens::UNINFORMED);
    Board b(size, State(size, 0));
    int sm = 0;
    int strat = 1;

    std::cout << "Initial State :" << std::endl;
    printGrid(b);
    algo.threadAlgo(&b, &sm, &strat);

    // algo.PrintBoard();
    std::cout << "Final State :" << std::endl;
    printGrid(b);
    std::cout << "Iterations    : " << algo.GetIterations() << std::endl;
    std::cout << "Time          : " << algo.GetTime() << " ms" << std::endl;

    return 0;
}