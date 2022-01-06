#include "Algorithm.hpp"

Algorithm::Algorithm(int n, STRAT strat)
{
    std::srand(std::time(nullptr));
    double intpart;
    if (modf(sqrt(n + 1), &intpart) == 0.0) {
        _n = n;
        _len = sqrt(n + 1);
    }
    _strategy = strat;
    _openList = new List;
    _closeList = new List;
}

Algorithm::~Algorithm()
{
}

int Algorithm::randomInt(std::vector<int> tab)
{
    int rand = std::rand() % (_n);
    while (std::find(tab.begin(), tab.end(), rand) != tab.end()) {
        rand += 1;
        if (rand > _n)
            rand = 0;
    }
    return (rand);
}

std::vector<int> Algorithm::createTab()
{
    std::vector<int> tab;
    for(int i = 0; i < _len; i++) {
        for(int j = 0; j < _len; j++) {
            int rand = randomInt(tab);
            tab.push_back(rand);
        }
    }
    return (tab);
}

bool Algorithm::validateTab()
{
    bool valid = true;

    for(int i = 0; i <= _n; i++) {
        if (_tab[i] != _finalConfig[i])
            valid = false;
    }

    return (valid);
}

int Algorithm::getDiff(std::vector<int> tab, std::vector<int> finalConfig)
{
    int score = 0;
    for(int i = 0; i <= _n; i++) {
        if (tab[i] != finalConfig[i])
            score++;
    }
    if (score > 0)
        score--;
    return score;
}

int Algorithm::getParentCost(std::vector<int> tab)
{
    Node *node = findNodeFromGrid(tab);
    if (node == NULL) {
        return (0);
    }
    Node *parent = node->parent;
    int score = 0;

    while (parent != NULL) {
        score += parent->score;
        parent = parent->parent;
    }
    return (score);
}

int Algorithm::computeScore(std::vector<int> tab, int score)
{
    if (score != 0)
        return (_iteration + getDiff(tab, _finalConfig) + score);
    return (_iteration + getDiff(tab, _finalConfig) + getParentCost(tab));
}

void Algorithm::possibility(std::vector<int> grid, int indexZero)
{
    if (indexZero >= _len && (std::find(_closedList.begin(), _closedList.end(), std::pair<int, std::pair<int, int>>(indexZero, std::pair<int, int>(indexZero - _len, grid[indexZero - _len]))) == _closedList.end())) { // CHECK TOP
        _indexAvailablePosition.push_back(std::pair<int, int>(indexZero - _len, grid[indexZero - _len]));
    }
    if (indexZero < _len * (_len - 1) && (std::find(_closedList.begin(), _closedList.end(), std::pair<int, std::pair<int, int>>(indexZero, std::pair<int, int>(indexZero + _len, grid[indexZero + _len]))) == _closedList.end())) { // CHECK BOTTOM
        _indexAvailablePosition.push_back(std::pair<int, int>(indexZero + _len, grid[indexZero + _len]));
    }
    if ((_len - 1 + _len * (indexZero / _len)) != indexZero && (std::find(_closedList.begin(), _closedList.end(), std::pair<int, std::pair<int, int>>(indexZero, std::pair<int, int>(indexZero + 1, grid[indexZero + 1]))) == _closedList.end())) { // CHECK RIGHT
        _indexAvailablePosition.push_back(std::pair<int, int>(indexZero + 1, grid[indexZero + 1]));
    }
    if ((_len * (indexZero / _len)) != indexZero && (std::find(_closedList.begin(), _closedList.end(), std::pair<int, std::pair<int, int>>(indexZero, std::pair<int, int>(indexZero - 1, grid[indexZero - 1]))) == _closedList.end())) { // CHECK LEFT
        _indexAvailablePosition.push_back(std::pair<int, int>(indexZero - 1, grid[indexZero - 1]));
    }
}

void Algorithm::findMovement(std::vector<int> grid)
{
    int indexZero = std::find(grid.begin(), grid.end(), 0) - grid.begin();
    _indexAvailablePosition.clear();

    possibility(grid, indexZero);

    if (_indexAvailablePosition.size() == 0) {
        _closedList.clear();
        possibility(grid, indexZero);
    }
}

bool Algorithm::isValid(std::vector<int> grid)
{
    bool valid = true;

    for(int i = 0; i <= _n; i++) {
        if (grid[i] != _finalConfig[i])
            valid = false;
    }

    return (valid);
}

void Algorithm::localAlgo()
{
    while (!validateTab()) {
        findMovement(_tab);
        int indexZero = std::find(_tab.begin(), _tab.end(), 0) - _tab.begin();
        std::pair<int, int> bestScore = std::pair<int, int>(-1, -1);
        int i = 0;
        // std::cout << "ALL REPRESENTATION" << std::endl;
        std::vector<int> cpy = _tab;
        for(auto pair: _indexAvailablePosition) {
            std::vector<int> tab = _tab;
            tab[pair.first] = 0;
            tab[indexZero] = pair.second;
            int score = computeScore(tab, 1);
            if ((bestScore.first == -1 && bestScore.second == -1) || score <= bestScore.second) {
                bestScore.second = score;
                bestScore.first = i;
            }
            i++;
            // std::cout << "SCORE = " << score << std::endl;
            // show(tab);
        }
        // std::cout << "END REPRESENTATION" << std::endl;
        _tab[_indexAvailablePosition[bestScore.first].first] = 0;
        _tab[indexZero] = _indexAvailablePosition[bestScore.first].second;
        _closedList.push_back(std::pair<int, std::pair<int, int>>(indexZero, std::pair<int, int>(_indexAvailablePosition[bestScore.first].first, _indexAvailablePosition[bestScore.first].second)));
        _iteration++;
        std::cout << std::endl << std::endl << "NEXT GENERATION WITH" << std::endl;
        show(_tab);
        addToOpenList(_tab, findNodeFromGrid(cpy));
        std::cout << std::endl << std::endl;
        std::cout << "GOAL = " << std::endl;
        std::cout << std::endl << std::endl;
        // if (_iteration > 10)
        //     break;
    }
    showList();
}

Algorithm::Node *Algorithm::getBestNode()
{
    if (_openList == NULL || _openList->node == NULL)
        return (NULL);
    Node *act = _openList->node;
    Node *bestNode = _openList->node;
    while(act != NULL) {
        if (bestNode == NULL || bestNode->score <= act->score)
            bestNode = act;
        act = act->next;
    }
    return (bestNode);
}

bool Algorithm::process(Algorithm::Node *node)
{
    _tab = node->grid;
    _iteration = 1;
    bool hasFound = true;
    while (!validateTab()) {
        findMovement(_tab);
        int indexZero = std::find(_tab.begin(), _tab.end(), 0) - _tab.begin();
        std::pair<int, int> bestScore = std::pair<int, int>(-1, -1);
        int i = 0;
        std::vector<int> cpy = _tab;
        for(auto pair: _indexAvailablePosition) {
            std::vector<int> tab = _tab;
            tab[pair.first] = 0;
            tab[indexZero] = pair.second;
            int score = computeScore(tab, node->score);
            if ((bestScore.first == -1 && bestScore.second == -1) || score <= bestScore.second) {
                bestScore.second = score;
                bestScore.first = i;
            }
            i++;
        }
        _tab[_indexAvailablePosition[bestScore.first].first] = 0;
        _tab[indexZero] = _indexAvailablePosition[bestScore.first].second;
        _closedList.push_back(std::pair<int, std::pair<int, int>>(indexZero, std::pair<int, int>(_indexAvailablePosition[bestScore.first].first, _indexAvailablePosition[bestScore.first].second)));
        addToOpenList(_tab, findNodeFromGrid(cpy));
        _iteration++;
        if (_iteration > 1000) {
            hasFound = false;
            break;
        }
    }
    // if (hasFound)
    show(_tab);
    return (hasFound);
}

void Algorithm::informedAlgo()
{
    while (_openList->node != NULL) {
        // findMovement(_tab);
        Node *bestNode = getBestNode();
        if (process(bestNode)) {
            break;
        }
        deleteFromOpenList(bestNode);
        if (bestNode->grid == _initialConfig) {
            
        }
        // INSERT TO CLOSE LIST ?
    }
    // showList();
}


void Algorithm::uninformedAlgo()
{
    _activeQueue.push_back(_tab);
    while (!validateTab()) {
        for(auto grid: _activeQueue) {
            findMovement(grid);
            int indexZero = std::find(grid.begin(), grid.end(), 0) - grid.begin();
            for(auto pair: _indexAvailablePosition) {
                std::vector<int> tab = grid;
                tab[pair.first] = 0;
                tab[indexZero] = pair.second;
                addToOpenList(tab, findNodeFromGrid(grid));
                if (isValid(tab))
                    return;
                _inactiveQueue.push_back(tab);
            }
        }
        _activeQueue.clear();
        _activeQueue = _inactiveQueue;
        _inactiveQueue.clear();
    }
    showList();
}

void Algorithm::showList()
{
    if (_openList == NULL) {
            exit(84);
        }

    Node *act = _openList->node;
    std::cout << "LINKED LIST -> " << std::endl;
    while (act != NULL) {
        std::cout << "[ grid: " << std::endl;
        show(act->grid);
        std::cout << std::endl << "Score: " << act->score << std::endl;
        std::cout << std::endl << "Parent: " << std::endl;
        if (act->parent != NULL)
            show(act->parent->grid);
        std::cout << "]" << std::endl;
        act = act->next;
    }
}

void Algorithm::writeFile(std::vector<int> tab, std::string file)
{
    std::ofstream myfile;
    myfile.open(file);
    int i = 0;
    int biggest = 0;

    for(auto nb: tab) {
        biggest = biggest > nb ? biggest : nb;
    }

    for(auto nb: tab) {
        if (i + 1 == _len) {
            myfile << std::left << nb << std::endl;
            i = 0;
            continue;
        } else if (i == 0) {
            myfile << std::left << std::setw(std::to_string(biggest).length()) << std::setfill(' ') << nb << " | " << std::setfill(' ');
            i++;
            continue;
        }
        myfile << std::left << std::setw(std::to_string(biggest).length()) << std::setfill(' ') << nb << " | " << std::setfill(' ');
        i++;
    }
    myfile.close();
}

void Algorithm::initList()
{
    List *list = new List;
    List *closed = new List;
    Node *node = new Node;

    if (_openList == NULL || node == NULL) {
        exit(84);
    }

    node->grid = _initialConfig;
    node->score = computeScore(_initialConfig);
    node->next = new Node;
    list->node = node;
    _openList = list;
    _closeList = closed;
}

void Algorithm::addToOpenList(std::vector<int> grid, Node *parent)
{
    Node *newOne = new Node;

    newOne->grid = grid;
    newOne->score = computeScore(grid);
    newOne->parent = parent;
    newOne->next = _openList->node;
    _openList->node = newOne;
}

void Algorithm::addToCloseList(std::vector<int> grid, Node *parent)
{
    Node *newOne = new Node;

    newOne->grid = grid;
    newOne->score = computeScore(grid);
    newOne->parent = parent;
    newOne->next = _closeList->node;
    _closeList->node = newOne;
}

Algorithm::Node *Algorithm::findNodeFromGrid(std::vector<int> grid)
{
    if (_openList == NULL) {
        exit(84);
    }

    Node *act = _openList->node;
    while (act != NULL) {
        if (act->grid == grid) {
            break;
        }
        act = act->next;
    }
    return (NULL);
}

void Algorithm::deleteFromOpenList(Node *bestNode)
{
    if (_openList == NULL) {
        exit(84);
    }

    if (bestNode != NULL) {
        Node *act = _openList->node;
        while (act != NULL) {
            if (act->parent == bestNode) {
                deleteFromOpenList(act);
            }
            act = act->next;
        }
        _openList->node = _openList->node->parent;
        free(bestNode);
    }
    // Node *act = _openList->node;
    // while (act != NULL) {
    //     if (act->parent == bestNode) {
    //         deleteFromOpenList(act);
    //     }
    //     act = act->next;
    // }
}

void Algorithm::deleteLastFromOpenList(Node *bestNode)
{
    if (_openList == NULL) {
        exit(84);
    }

    if (bestNode != NULL) {
        Node *act = _openList->node;
        while (act != NULL) {
            if (act->parent == bestNode) {
                deleteFromOpenList(act);
            }
            act = act->next;
        }
        _openList->node = _openList->node->parent;
        free(bestNode);
    }
    // Node *act = _openList->node;
    // while (act != NULL) {
    //     if (act->parent == bestNode) {
    //         deleteFromOpenList(act);
    //     }
    //     act = act->next;
    // }
}

void Algorithm::compute()
{
    if (_n == 0 && _len == 0)
        return;
    // _initialConfig = createTab();
    // _finalConfig = createTab();
    _initialConfig.push_back(2);
    _initialConfig.push_back(3);
    _initialConfig.push_back(4);
    _initialConfig.push_back(0);
    _initialConfig.push_back(1);
    _initialConfig.push_back(5);
    _initialConfig.push_back(6);
    _initialConfig.push_back(7);
    _initialConfig.push_back(10);
    _initialConfig.push_back(11);
    _initialConfig.push_back(12);
    _initialConfig.push_back(8);
    _initialConfig.push_back(9);
    _initialConfig.push_back(13);
    _initialConfig.push_back(14);
    _initialConfig.push_back(15);
    
    _finalConfig.push_back(1);
    _finalConfig.push_back(2);
    _finalConfig.push_back(3);
    _finalConfig.push_back(4);
    _finalConfig.push_back(5);
    _finalConfig.push_back(6);
    _finalConfig.push_back(7);
    _finalConfig.push_back(8);
    _finalConfig.push_back(9);
    _finalConfig.push_back(10);
    _finalConfig.push_back(11);
    _finalConfig.push_back(12);
    _finalConfig.push_back(13);
    _finalConfig.push_back(14);
    _finalConfig.push_back(15);
    _finalConfig.push_back(0);

    initList();
    writeFile(_initialConfig, "init");
    writeFile(_finalConfig, "end");
    // showInitialConfig();
    _tab = _initialConfig;
    switch (_strategy) {
        case STRAT::INFORMED:
            informedAlgo();
            break;
        case STRAT::UNIFORMED:
            uninformedAlgo();
            break;
        case STRAT::LOCAL:
            localAlgo();
            break;
        default:
            break;
        }
    
}

void Algorithm::show(std::vector<int> tab)
{
    std::cout << "NOW configuration:" << std::endl;
    int i = 0;
    int biggest = 0;

    for(auto nb: tab) {
        biggest = biggest > nb ? biggest : nb;
    }

    for(auto nb: tab) {
        if (i + 1 == _len) {
            std::cout << std::left << nb << std::endl;
            i = 0;
            continue;
        } else if (i == 0) {
            std::cout << std::left << std::setw(std::to_string(biggest).length()) << std::setfill(' ') << nb << " | " << std::setfill(' ');
            i++;
            continue;
        }
        std::cout << std::left << std::setw(std::to_string(biggest).length()) << std::setfill(' ') << nb << " | " << std::setfill(' ');
        i++;
    }
}

void Algorithm::showInitialConfig()
{
    std::cout << "Initial configuration:" << std::endl;
    int i = 0;
    int biggest = 0;

    for(auto nb: _initialConfig) {
        biggest = biggest > nb ? biggest : nb;
    }

    for(auto nb: _initialConfig) {
        if (i + 1 == _len) {
            std::cout << std::left << nb << std::endl;
            i = 0;
            continue;
        } else if (i == 0) {
            std::cout << std::left << std::setw(std::to_string(biggest).length()) << std::setfill(' ') << nb << " | " << std::setfill(' ');
            i++;
            continue;
        }
        std::cout << std::left << std::setw(std::to_string(biggest).length()) << std::setfill(' ') << nb << " | " << std::setfill(' ');
        i++;
    }
}

void Algorithm::showFinalConfig()
{
    std::cout << "Final configuration:" << std::endl;
    int i = 0;
    int biggest = 0;

    for(auto nb: _finalConfig) {
        biggest = biggest > nb ? biggest : nb;
    }

    for(auto nb: _finalConfig) {
        if (i + 1 == _len) {
            std::cout << std::left << nb << std::endl;
            i = 0;
            continue;
        } else if (i == 0) {
            std::cout << std::left << std::setw(std::to_string(biggest).length()) << std::setfill(' ') << nb << " | " << std::setfill(' ');
            i++;
            continue;
        }
        std::cout << std::left << std::setw(std::to_string(biggest).length()) << std::setfill(' ') << nb << " | " << std::setfill(' ');
        i++;
    }
}