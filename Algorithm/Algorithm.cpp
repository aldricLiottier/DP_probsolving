#include "Algorithm.hpp"

Algorithm::Algorithm(int n, STRAT strategy, bool oneStep)
{
    std::srand(std::time(nullptr));
    double intpart;
    if (modf(sqrt(n + 1), &intpart) == 0.0) {
        _n = n;
        _len = sqrt(n + 1);
    }
    _strategy = strategy;
    _openList = new List;
    _closeList = new List;

    tabGui = NULL;
    state = NULL;
    strat = NULL;
    _oneStep = oneStep;
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

void Algorithm::threadAlgo(std::vector<std::vector<int>> *grid, int *state, int *strat)
{
    auto time0 = std::chrono::high_resolution_clock::now();
    tabGui = grid;
    this->state = state;

    initList();
    std::vector<int> tab;
    for(auto vec: *grid) {
        for(auto nb: vec) {
            tab.push_back(nb);
        }
    }
    _initialConfig = tab;
    _tab = _initialConfig;
    writeFile(_initialConfig, "init");
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
    auto time1 = std::chrono::high_resolution_clock::now();
    this->ms = std::chrono::duration_cast<std::chrono::milliseconds>(time1 - time0).count();
}

std::vector<int> Algorithm::convertIntToVector(int **tab)
{
    std::vector<int> table;

    for(int i = 0; i < _len; i++) {
        for(int j = 0; j < _len; j++) {
            table.push_back(tab[i][j]);
        }
    }
    return (table);
}

std::vector<std::vector<int>> Algorithm::convertVectorToVector(std::vector<int> grid)
{
    std::vector<std::vector<int>> tab;
    int i = 0;
    int j = 0;

    for(auto nb: grid) {
        if (i == _len) {
            i = 0;
            j++;
        }
        tab[j].push_back(nb);
        i++;
    }
    return (tab);
}

std::vector<int> Algorithm::createTab() // Fonction générateur de grille, random, generateur, à utiliser avec la méthode de convert de vector
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

template<typename T>
int Algorithm::algoProcess(SNode &finalNode, std::set<SNode> &closedNode, T &list) {
    int iter = 0;
    while (!list.empty()) {
        SNode next = list.top();
        list.pop();
        if (next.isFinished()) {
            finalNode = next;
            return iter;
        }
        if (closedNode.empty() || closedNode.find(next) == closedNode.end()) {
            closedNode.insert(next);
            std::vector<SNode> children = next.toNext();
            for (SNode &s : children) {
                list.push(s);
            }
        }
        iter++;
        if (_oneStep) {
            show(convertIntToVector(next.getTiles()));
            std::string str; 
            std::getline( std::cin, str);
        }
    }
    return -1;
}

int Algorithm::depthFirstSearch(SNode &finalNode, std::set<SNode> &closedNode, std::stack<SNode> &list) {
    return algoProcess(finalNode, closedNode, list);
}

int Algorithm::bestFirstSearch(SNode &finalNode, std::set<SNode> &closedNode, OwnPrioQueue &list) {
    return algoProcess(finalNode, closedNode, list);
}

void Algorithm::informedAlgo() // Best First search algorithm
{
    bool solvable = false;
    SNode start;
    while (!solvable) {
        start = SNode(_len, _len, _tab);
        solvable = start.canBeDone();
        if (!solvable) {
            _initialConfig = createTab();
            _tab = _initialConfig;
        }
    }
    std::cout << "Initial Config: " << std::endl << start.toString() << std::endl;

    int iterations;
    auto time0 = std::chrono::high_resolution_clock::now();
    SNode end;
    std::set<SNode> closedSet;

    OwnPrioQueue agenda;
    agenda.push(start);
    
    iterations = bestFirstSearch(end, closedSet, agenda);

    auto time1 = std::chrono::high_resolution_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(time1 - time0).count();

    std::cout << "Solution:" << std::endl;
    std::vector<std::string> path = end.getPath();
    this->iteration = iterations;
    this->nodeSize = path.size();

    if (!_oneStep) {
        std::vector<std::vector<int>> endVector;
        endVector.push_back(convertIntToVector(end.getTiles()));
        SNode *ptr = end.getPrevious();

        do {
            endVector.push_back(convertIntToVector(ptr->getTiles()));
            ptr = ptr->getPrevious();
        } while (ptr != nullptr);

        std::reverse(endVector.begin(), endVector.end());

        for(auto grid: endVector) {
            show(grid);
            std::cout << std::endl;
        }
    }

    std::cout << "Steps = " << path.size() - 1 << std::endl
                << "Iterations = " << iterations << std::endl
                << "Time = " << ms << " ms" << std::endl;
}

long long Algorithm::getTime()
{
    return (ms);
}

long long Algorithm::getNodeSize()
{
    return (nodeSize);
}

long long Algorithm::getIteration()
{
    return (iteration);
}

void Algorithm::uninformedAlgo() // Depth first search algorithm
{
    bool solvable = false;
    SNode start;
    while (!solvable) {
        start = SNode(_len, _len, _tab);
        solvable = start.canBeDone();
        if (!solvable) {
            _initialConfig = createTab();
            _tab = _initialConfig;
        }
    }
    std::cout << "Initial Config: " << std::endl << start.toString() << std::endl;

    int iterations;
    auto time0 = std::chrono::high_resolution_clock::now();
    
    SNode end;
    std::set<SNode> closedSet;

    std::stack<SNode> agenda;
    agenda.push(start);
    iterations = depthFirstSearch(end, closedSet, agenda);

    auto time1 = std::chrono::high_resolution_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(time1 - time0).count();

    std::cout << "Solution:" << std::endl;
    std::vector<std::string> path = end.getPath();
    // for (std::string &s : end.getPath()) {
    //     std::cout << s << std::endl;
    // }
    this->iteration = iterations;
    this->nodeSize = path.size();
    std::vector<std::vector<int>> endVector;
    endVector.push_back(convertIntToVector(end.getTiles()));
    SNode *ptr = end.getPrevious();

    do {
        endVector.push_back(convertIntToVector(ptr->getTiles()));
        ptr = ptr->getPrevious();
    } while (ptr != nullptr);

    std::reverse(endVector.begin(), endVector.end());

    for(auto grid: endVector) {
        show(grid);
        std::cout << std::endl;
    }

    std::cout << "Steps = " << path.size() - 1 << std::endl
            << "Iterations = " << iterations << std::endl
            << "Time = " << ms << " ms" << std::endl;
}

void Algorithm::localAlgo()
{
    Node *act = _openList->node;
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
            int score = computeScore(tab, 1);
            if ((bestScore.first == -1 && bestScore.second == -1) || score <= bestScore.second) {
                bestScore.second = score;
                bestScore.first = i;
            }
            i++;
        }
        _tab[_indexAvailablePosition[bestScore.first].first] = 0;
        _tab[indexZero] = _indexAvailablePosition[bestScore.first].second;
        
        _closedList.push_back(std::pair<int, std::pair<int, int>>(indexZero, std::pair<int, int>(_indexAvailablePosition[bestScore.first].first, _indexAvailablePosition[bestScore.first].second)));
        _iteration++;
        addToOpenList(_tab, findNodeFromGrid(cpy));
        if (_oneStep) {
            show(_tab);
            std::string str; 
            std::getline( std::cin, str);
        }
    }
    showPath();
    // showList();
}

void Algorithm::showPath()
{
    Node *act = findNodeFromGrid(_tab);
    std::vector<std::vector<int>> vec;
    while (act != NULL) {
        vec.push_back(act->grid);
        act = act->parent;
    }
    vec.back();
    for(int i = vec.size() - 1; i >= 0; i--) {
        show(vec[i]);
        std::cout << std::endl;
    }
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

Algorithm::Node *Algorithm::addToOpenList(std::vector<int> grid, Node *parent)
{
    Node *newOne = new Node;

    newOne->grid = grid;
    newOne->score = computeScore(grid);
    newOne->parent = parent;
    // if (parent != NULL) {
    //     show(parent->grid);
    //     std::cout << std::endl;
    //     show(grid);
    //     std::cout << std::endl;
    // }
    newOne->next = _openList->node;
    _openList->node = newOne;
    return (newOne);
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
            return (act);
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
    _initialConfig = createTab();

    // To test with n = 8 the local algorithm since it can't run complex puzzle, only simple like this one is do-able

    // _initialConfig.push_back(0);
    // _initialConfig.push_back(1);
    // _initialConfig.push_back(2);
    // _initialConfig.push_back(5);
    // _initialConfig.push_back(6);
    // _initialConfig.push_back(3);
    // _initialConfig.push_back(4);
    // _initialConfig.push_back(7);
    // _initialConfig.push_back(8);

    //

    _finalConfig.push_back(1);
    _finalConfig.push_back(2);
    _finalConfig.push_back(3);
    _finalConfig.push_back(4);
    _finalConfig.push_back(5);
    _finalConfig.push_back(6);
    _finalConfig.push_back(7);
    _finalConfig.push_back(8);
    _finalConfig.push_back(0);
    
    initList();
    // writeFile(_initialConfig, "init"); // To write a npuzzle grid in a file
    // writeFile(_finalConfig, "end");
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
    int i = 0;
    int biggest = 0;

    for(auto nb: tab) {
        biggest = biggest > nb ? biggest : nb;
    }

    for(auto nb: tab) {
        if (i + 1 == _len) {
            std::cout << std::left << (nb == 0 ? "\033[31m0\u001b[0m" : std::to_string(nb)) << std::endl;
            i = 0;
            continue;
        } else if (i == 0) {
            std::cout << std::left << std::setw(std::to_string(biggest).length()) << std::setfill(' ') << (nb == 0 ? "\033[31m0\u001b[0m" : std::to_string(nb)) << " | " << std::setfill(' ');
            i++;
            continue;
        }
        std::cout << std::left << std::setw(std::to_string(biggest).length()) << std::setfill(' ') << (nb == 0 ? "\033[31m0\u001b[0m" : std::to_string(nb)) << " | " << std::setfill(' ');
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