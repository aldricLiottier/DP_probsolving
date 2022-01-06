#include "Algorithm/Algorithm.hpp"

int main(int ac, char const **av)
{
    if (ac != 2)
        return (84);

    Algorithm algo(std::atoi(av[1]), Algorithm::STRAT::UNIFORMED);
    algo.compute();
    algo.showInitialConfig();
    algo.showFinalConfig();
    // Algorithm::List *list = new Algorithm::List;
    // Algorithm::Node *h = list->node;
    return (0);
}
