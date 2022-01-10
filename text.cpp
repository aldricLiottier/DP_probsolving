#include "text.hpp"

textsf::textsf(std::string content, sf::RenderWindow *windows, textsf *firsty) : text(), pos(20, 20)
{
    window = windows;
    first = firsty;
    currenttxt = first;
    if (!font.loadFromFile("KIMONOK_.ttf"))
    {
        throw EXIT_FAILURE;
    }
    // select the font
    text.setFont(font); // font is a sf::Font

    // set the string to display
    text.setString(content);

    // set the character size
    text.setCharacterSize(22); // in pixels, not points!

    text.setPosition(pos);
    // set the color
    text.setFillColor(sf::Color::Red);

    // set the text style
    text.setStyle(sf::Text::Bold | sf::Text::Underlined);

    // inside the main loop, between window.clear() and window.display()
    window->draw(text);
}

void textsf::textsfDown()
{
    if (currenttxt->next != nullptr) {
        currenttxt->text.setFillColor(sf::Color::White);
        currenttxt = currenttxt->next;
        currenttxt->text.setFillColor(sf::Color::Red);
    } else {
        currenttxt->text.setFillColor(sf::Color::White);
        currenttxt = first;
        currenttxt->text.setFillColor(sf::Color::Red);
    }
}

void textsf::textsfUp()
{
    if (currenttxt->prev != nullptr) {
        currenttxt->text.setFillColor(sf::Color::White);
        currenttxt = currenttxt->prev;
        currenttxt->text.setFillColor(sf::Color::Red);
    } else {
        currenttxt->text.setFillColor(sf::Color::White);
        while (currenttxt->next != nullptr)
        {
            currenttxt = currenttxt->next;
        }
        currenttxt->text.setFillColor(sf::Color::Red);
    }
}

textsf::textsf()
{}

void textsf::drawall()
{
    text.setPosition(pos);
    window->draw(text);
    if (next != nullptr) {
        next->drawall();
    }

}

void textsf::newfoltext(std::string content, void (*addactivity)(sf::RenderWindow *window))
{
    if (next == nullptr) {
        next = new textsf(content, window, first);
        next->pos.y = pos.y + 25;
        next->prev = this;
        next->text.setFillColor(sf::Color::White);
        next->activity = addactivity;
    } else {
        textsf *temp;
        temp = next;
        while (temp->next != nullptr)
        {
            temp = temp->next;
        }
        temp->next = new textsf(content, window, first);
        temp->next->pos.y = temp->pos.y + 25;
        temp->next->prev = temp;
        temp->next->text.setFillColor(sf::Color::White);
        temp->next->activity = addactivity;
    }
}

textsf::~textsf()
{
}