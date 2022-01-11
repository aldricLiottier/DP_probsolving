#ifndef AI1_DATASTRUCTS_H
#define AI1_DATASTRUCTS_H


#include <queue>
#include <set>
#include <stack>
#include "SNode.hpp"

struct compareSNodes {
    bool operator()(SNode &a, SNode &b) {
        return a.inPlace() < b.inPlace();
    }
};

class my_queue : public std::queue<SNode> {
public:
    SNode top() {
        return front();
    }
};

class OwnPrioQueue : public std::priority_queue<SNode, std::vector<SNode>, compareSNodes> {
};

#endif //AI1_DATASTRUCTS_H
