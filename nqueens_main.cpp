//
// Created by Utilisateur on 10/01/2022.
//

#include "NQueens.hpp"
#include <iostream>

#define ARGUMENT_ERROR -1
#define ERROR 84

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

    algo.Compute(NQueens::UNINFORMED);

    algo.PrintBoard();

    return 0;
}