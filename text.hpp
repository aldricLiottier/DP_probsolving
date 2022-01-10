#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <stdexcept>

class textsf
{
private:
    /* data */
    sf::RenderWindow *window;
public:
    textsf(std::string content, sf::RenderWindow *window, textsf *firsty);
    void (*activity)(sf::RenderWindow *window);
    void newfoltext(std::string, void (*addactivity)(sf::RenderWindow *window));
    void textsfDown();
    void textsfUp();
    void drawall();
    textsf();
    sf::Font font;
    sf::Vector2f pos;
    textsf *first = nullptr;
    textsf *currenttxt;
    textsf *next = nullptr;
    textsf *prev = nullptr;
    sf::Text text;
    ~textsf();
};

