#include "Algorithm/Algorithm.hpp"

int main(int ac, char const **av)
{
    if (ac != 2)
        return (84);
    
    std::string in;

    std::cout << "Would you like to have the One Step config ? [y/N]" << std::endl;
    std::cin >> in;
    bool oneStep = false;
    if (std::tolower(in[0]) == 'y') {
        oneStep = true;
    }

    in.clear();

    std::cout << "Which algorithm strategy would you like to try ?" << std::endl
                << " - Informed Search Strategy [1] (default)" << std::endl
                << " - Uninformed Search Strategy [2]" << std::endl
                << " - Local Search Strategy [3]" << std::endl;
    std::cin >> in;

    Algorithm::STRAT strat = Algorithm::STRAT::INFORMED;

    switch (std::stoi(in)) {
        case 2:
            strat = Algorithm::STRAT::UNIFORMED;
            break;
        case 3:
            strat = Algorithm::STRAT::LOCAL;
            break;
        default:
            break;
    }

    Algorithm algo(std::atoi(av[1]), strat, oneStep);
    algo.compute();
    // algo.showInitialConfig();
    // algo.showFinalConfig();
    // Algorithm::List *list = new Algorithm::List;
    // Algorithm::Node *h = list->node;
    return (0);
}
