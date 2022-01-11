#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <stdexcept>
#include "Algorithm.hpp"
#include <math.h>
#include <string>
#include <vector>
#include <sstream>
#include "NQueens.hpp"

class textsf
{
private:
    /* data */
    sf::RenderWindow *window;
public:
    textsf(std::string content, sf::RenderWindow *window, textsf *firsty);
    void (*activity)(sf::RenderWindow *window);
    void newfoltext(std::string, void (*addactivity)(sf::RenderWindow *window));
    void setState();
    bool unsetState();
    void textsfDown();
    void textsfUp();
    void drawall();
    textsf();
    void *board;
    void *Puzzlealgo;
    std::string textcontent;
    sf::Font font;
    sf::Vector2f pos;
    bool currentinuse = 0;
    textsf *algochoice = nullptr;
    textsf *first = nullptr;
    textsf *currenttxt;
    textsf *next = nullptr;
    textsf *prev = nullptr;
    sf::Text text;
    ~textsf();
};

#define IMGSIZE sf::VideoMode::getDesktopMode().width / 3

class sfimage
{
private:
    sf::RenderWindow *window;
public:
    sfimage(sf::RenderWindow *window);
    void addqueen();
    void addpuzzle(std::vector<std::vector<int>> &grid, std::vector<std::vector<int>> &grid2);
    int createcorrect(std::string name);
    std::vector<std::vector<int>> *initialstate;
    std::vector<std::vector<int>> *finalstate;
    sf::Texture backtexture;
    sf::Texture damiertexture;
    sf::Texture puzunsoltexture;
    sf::Texture puzsoltexture;
    sf::Image damierqueen;
    sf::Image puzzle;
    sf::Image damierpuzzle;
    sf::Image qfont;
    sf::Image nfont[10];
    //w 15 300 h 8 160 t 20
    sf::Sprite backsprite;
    sf::Sprite puzunsolsprite;
    sf::Sprite puzsolsprite;
    sf::Sprite damiersprite;
    sf::Image background;
    int damierspaces;
    int damiersquare;
    int damiersize;
    int puzspaces;
    int puzsquare;
    int puzsize;
    sf::Image Damier;
    sf::Image puzunsol;
    sf::Image puzsol;
    void drawall(textsf *);
    void createdam(int size);
    void createpuz(int size);

    ~sfimage();
};

