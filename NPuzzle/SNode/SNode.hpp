#ifndef AI1_STATE_H
#define AI1_STATE_H


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

        void moveBlank(Coords c);

        void moveX(int);

        void moveY(int);

        bool moveUp(SNode &);

        bool moveDown(SNode &);

        bool moveLeft(SNode &);

        bool moveRight(SNode &);

    public:

        SNode();

        SNode(int, int, std::vector<std::vector<int>>);

        SNode(int, int, int*);

        SNode(const SNode &);

        ~SNode();

        int **getTiles();

        SNode &operator=(const SNode &);

        friend bool operator<(const SNode &a, const SNode &b) {
            return a.toString() < b.toString();
        }

        bool canBeDone();

        bool isFinished();

        int manhattan();

        int inPlace();

        std::string toString() const;

        std::vector<SNode> expand();

        std::vector<std::string> getPath();
};


#endif //AI1_STATE_H
