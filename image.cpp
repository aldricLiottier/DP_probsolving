#include "image.hpp"


sfimage::sfimage(sf::RenderWindow *windows) : background(), Damier()
{
    window = windows;
    if (!backtexture.loadFromFile("dark-grey.jpg"))
        throw EXIT_FAILURE;
    backsprite.setTexture(backtexture, true);
    
}

void sfimage::drawall()
{
    window->draw(backsprite);
}

void sfimage::createdams(int size)
{
    Damier.create(IMGSIZE, IMGSIZE, sf::Color::Black);
    int spaces = 5 * size;
    int squaresize = (IMGSIZE - spaces) / size;
    spaces = spaces / size;
    damierspaces = spaces;
    damiersquare = squaresize;
    damiersize = size;
    sf::Image squareWhite;
    sf::Image squareRed;
    squareWhite.create(squaresize, squaresize, sf::Color::White);
    squareRed.create(squaresize, squaresize, sf::Color::Red);
    int tempwidth = spaces;
    int tempheight = spaces;
    for (size_t i = 0; i < size; i++)
    {
        for (size_t n = 0; n < size; n++)
        {
            if (n%2 == 0)
                Damier.copy(squareWhite, tempwidth, tempheight);
            else
                Damier.copy(squareRed, tempwidth, tempheight);
            tempwidth += squaresize + spaces;
        }
        tempwidth = spaces;
        tempheight += squaresize + spaces;
    }
    Damier.saveToFile("result.png");
}

sfimage::~sfimage()
{
}