#ifndef SNODE_HPP
#define SNODE_HPP


#include <vector>
#include <string>

struct Coords {
    int height, width;
};

class SNode {
    private:
        int **tiles;
        Coords blank;
        int width, height, size;
        SNode* previous;

        bool moveUp(SNode &);
        bool moveDown(SNode &);
        bool moveLeft(SNode &);
        bool moveRight(SNode &);
        void moveBlank(Coords c);
        void moveX(int);
        void moveY(int);

    public:

        SNode();
        SNode(int, int, std::vector<int>);
        SNode(int, int, int*);
        SNode(const SNode &);
        ~SNode();
        int **getTiles();
        SNode &operator=(const SNode &);
        bool canBeDone();
        bool isFinished();
        int manhattan();
        int inPlace();
        std::string toString() const;
        std::vector<SNode> toNext();
        friend bool operator<(const SNode &a, const SNode &b) {
            return a.toString() < b.toString();
        }
        std::vector<std::string> getPath();
        SNode* getPrevious();
};


#endif
