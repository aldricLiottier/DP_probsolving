#include <chrono>
#include <algorithm>
#include <random>
#include <sstream>
#include <iostream>
#include "SNode.hpp"

SNode::SNode() {
    tiles = nullptr;
    previous = nullptr;
    width = 0;
    height = 0;
    size = 0;
    blank = {0, 0};
}

SNode::SNode(int w, int h, std::vector<std::vector<int>> grid) {
    auto numbers = new int[w * h];
    int k = 0;
    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            numbers[k] = grid[i][j];
            k++;
        }
    }

    // long seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    // std::shuffle(&numbers[0], &numbers[w * h], std::default_random_engine(seed));

    *this = SNode(w, h, numbers);
    delete[] numbers;
}

SNode::SNode(int w, int h, int* numbers) {
    width = w;
    height = h;
    size = w * h;
    previous = nullptr;

    tiles = new int *[height];
    for (int i = 0; i < height; i++) {
        tiles[i] = new int[width];

        for (int j = 0; j < width; j++) {
            tiles[i][j] = numbers[i * width + j];

            if (tiles[i][j] == 0) {
                blank.height = i;
                blank.width = j;
            }
        }
    }
}

SNode::SNode(const SNode &s) {
    width = s.width;
    height = s.height;
    size = s.size;
    previous = s.previous;

    blank = s.blank;
    tiles = new int *[height];
    for (int i = 0; i < height; i++) {
        tiles[i] = new int[width];

        for (int j = 0; j < width; j++) {
            tiles[i][j] = s.tiles[i][j];
        }
    }
}

SNode::~SNode() {
    delete[] tiles;
}

int **SNode::getTiles()
{
    return (this->tiles);
}

std::string SNode::toString() const {
    long maxLength = std::to_string(size - 1).length();

    std::stringstream out;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int num = tiles[i][j];
            if (num == 0) {
                for (int k = 0; k < maxLength; k++) {
                    out << "_";
                }
                out << " ";
            } else {
                long numLength = std::to_string(num).length();
                for (long k = numLength; k < maxLength; k++) {
                    out << " ";
                }
                out << num << " ";
            }
        }
        out << std::endl;
    }
    return out.str();
}

SNode &SNode::operator=(const SNode &o) {
    width = o.width;
    height = o.height;
    size = o.size;
    previous = o.previous;

    blank = o.blank;
    tiles = new int *[height];
    for (int i = 0; i < height; i++) {
        tiles[i] = new int[width];

        for (int j = 0; j < width; j++) {
            tiles[i][j] = o.tiles[i][j];
        }
    }

    return *this;
}

bool SNode::canBeDone() {
    int swaps = 0;
    int flatTiles[size];

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            flatTiles[height * i + j] = tiles[i][j];
        }
    }

    for (int i = 0; i < size; i++) {
        int tile = flatTiles[i];
        if (tile == 0) continue;

        for (int j = i + 1; j < size; j++) {
            int nextTile = flatTiles[j];
            if (nextTile == 0) continue;

            if (tile > nextTile) {
                swaps++;
            }
        }
    }

    bool evenSwaps = swaps % 2 == 0;
    bool blankOddRow = (height - blank.height) % 2 == 1;

    return width == 0 ? blankOddRow == evenSwaps : evenSwaps;
}

bool SNode::isFinished() {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int expected = 1 + i * width + j;
            int actual = tiles[i][j];

            if (expected == size) {
                if (actual != 0) {
                    return false;
                }
            } else {
                if (actual != expected) {
                    return false;
                }
            }
        }
    }
    return true;
}

int SNode::manhattan() {
    int score = 0;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int value = tiles[i][j];
            int posX = value / width;
            int posY = value % width;
            score += abs(posX - i) + abs(posY - j);
        }
    }
    return score;
}

int SNode::inPlace() {
    int score = 0;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int value = tiles[i][j];
            int expected = 1 + i * width + j;
            if (value == expected || expected == size)
                score++;
        }
    }
    return score;
}

std::vector<SNode> SNode::expand() {
    std::vector<SNode> children;

    SNode s;
    if (moveUp(s))
        children.push_back(s);

    if (moveDown(s))
        children.push_back(s);

    if (moveLeft(s))
        children.push_back(s);

    if (moveRight(s))
        children.push_back(s);

    return children;
}

void SNode::moveBlank(Coords c) {
    Coords moveFrom = blank;
    blank.height += c.height;
    blank.width += c.width;

    int num = tiles[blank.height][blank.width];
    tiles[moveFrom.height][moveFrom.width] = num;
    tiles[blank.height][blank.width] = 0;
}

void SNode::moveX(int x) {
    moveBlank({x, 0});
}

void SNode::moveY(int y) {
    moveBlank({0, y});
}

bool SNode::moveUp(SNode &s) {
    if (blank.height == 0) {
        return false;
    }

    s = *this;
    s.moveX(-1);
    s.previous = new SNode(*this);
    return true;
}

bool SNode::moveDown(SNode &s) {
    if (blank.height == height - 1) {
        return false;
    }

    s = *this;
    s.moveX(+1);
    s.previous = new SNode(*this);
    return true;
}

bool SNode::moveLeft(SNode &s) {
    if (blank.width == 0) {
        return false;
    }

    s = *this;
    s.moveY(-1);
    s.previous = new SNode(*this);
    return true;
}

bool SNode::moveRight(SNode &s) {
    if (blank.width == width - 1) {
        return false;
    }

    s = *this;
    s.moveY(+1);
    s.previous = new SNode(*this);
    return true;
}

std::vector<std::string> SNode::getPath() {
    if (previous == nullptr) {
        return std::vector<std::string>();
    }

    std::vector<std::string> path;
    path.emplace_back(toString());

    SNode* ptr = previous;
    do {
        path.emplace_back(ptr->toString());
        ptr = ptr->previous;
    } while (ptr != nullptr);

    std::reverse(path.begin(), path.end());
    return path;
}
