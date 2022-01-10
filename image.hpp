#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <stdexcept>
#define IMGSIZE sf::VideoMode::getDesktopMode().width / 3

class sfimage
{
private:
    sf::RenderWindow *window;
public:
    sfimage(sf::RenderWindow *window);
    sf::Texture backtexture;
    sf::Texture damiertexture;
    sf::Sprite backsprite;
    sf::Sprite damiersprite;
    sf::Image background;
    int damierspaces;
    int damiersquare;
    int damiersize;
    sf::Image Damier;
    void drawall();
    void createdams(int size);

    ~sfimage();
};

