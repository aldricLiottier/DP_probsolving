#include <math.h>
#include <random>
#include <vector>
#include <ctime>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <fstream>
#include <chrono>
#include <map>
#include <string>
#include <thread>
#include <mutex>
#include "DataStruct.hpp"

class Algorithm
{

    public:
        enum STRAT {
            INFORMED,
            UNIFORMED,
            LOCAL
        };
        struct Node
        {
            std::vector<int> grid;
            int score;
            Node *parent;
            Node *next;
        };

        struct List
        {
            Node *node;
        };

        Algorithm(int n, STRAT strategy = STRAT::INFORMED);
        ~Algorithm();
        void compute();
        int randomInt(std::vector<int> tab);
        void showInitialConfig();
        void showFinalConfig();
        void show(std::vector<int> tab);
        void showList();
        void threadAlgo(std::vector<std::vector<int>> *grid, int *state, int *strat);
    private:
        std::vector<int> createTab();
        void localAlgo();
        void informedAlgo();
        void uninformedAlgo();
        bool validateTab();
        void findMovement(std::vector<int> grid);
        int computeScore(std::vector<int> tab, int score = 0);
        int getDiff(std::vector<int> tab, std::vector<int> finalConfig);
        void possibility(std::vector<int> grid, int indexZero);
        void writeFile(std::vector<int> tab, std::string file);
        Node *addToOpenList(std::vector<int> grid, Node *parent);
        void addToCloseList(std::vector<int> grid, Node *parent);
        void initList();
        void deleteFromOpenList(Node *bestNode);
        void deleteLastFromOpenList(Node *bestNode);
        Node *findNodeFromGrid(std::vector<int> grid);
        bool isValid(std::vector<int> grid);
        Node *getBestNode();
        int getParentCost(std::vector<int> tab);
        bool process(Algorithm::Node *node);
        void showPath();
        std::vector<int> convertIntToVector(int **tab);
        std::vector<std::vector<int>> convertVectorToVector(std::vector<int> grid);
        int depthFirstSearch(SNode &finalNode, std::set<SNode> &closedNode, std::stack<SNode> &list);
        int bestFirstSearch(SNode &finalNode, std::set<SNode> &closedNode, OwnPrioQueue &list);
        template<typename T>
        int algoProcess(SNode &finalNode, std::set<SNode> &closedNode, T &list);
        // std::vector<int> fill_A_Config();
        // std::vector<int> fill_B_Config();
        // void checkWhichConfig();
        long long getTime();
        long long getNodeSize();
        long long getIteration();

    private:
        int _n = 0;
        int _len = 0;
        std::vector<int> _tab;
        std::vector<int> _initialConfig;
        std::vector<int> _finalConfig;
        // std::vector<int> _A_finalConfig;
        // std::vector<int> _B_finalConfig;
        STRAT _strategy = STRAT::INFORMED;
        std::vector<std::pair<int, int>> _indexAvailablePosition;
        int _iteration = 1;
        std::pair<int, int> _postionBefore;
        std::vector<std::pair<int, std::pair<int, int>>> _closedList;
        List *_openList;
        List *_closeList;
        std::vector<std::vector<int>> _activeQueue;
        std::vector<std::vector<int>> _inactiveQueue;

        std::vector<std::vector<int>> *tabGui;
        int *state;
        int *strat;
        std::mutex _algoMutex;
        long long ms;
        long long nodeSize;
        long long iteration;
};
