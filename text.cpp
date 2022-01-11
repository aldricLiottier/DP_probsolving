#include "iostream"
#include "gui.hpp"

textsf::textsf(std::string content, sf::RenderWindow *windows, textsf *firsty) : text(), pos(20, 20), textcontent(content)
{
    window = windows;
    if (content != "informed search" && content != "uninformed search" && content != "local search" &&
        content != "one step" && content != "full solve" && content != "reset") {
        algochoice = new textsf("informed search", windows, nullptr);
        algochoice->first = algochoice;
        algochoice->currenttxt = algochoice;
        algochoice->newfoltext("uninformed search", nullptr);
        algochoice->newfoltext("local search", nullptr);
    } else if (content != "one step" && content != "full solve" && content != "reset" &&
                content != "8-puzzle" && content != "24-puzzle" && content != "20-Queens" && content != "1000000-Queens") {
        algochoice = new textsf("one step", windows, nullptr);
        algochoice->first = algochoice;
        algochoice->currenttxt = algochoice;
        algochoice->newfoltext("full solve", nullptr);
        algochoice->newfoltext("reset", nullptr);
    }
    if (firsty == nullptr)
        first = this;
    else 
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

void textsf::setState()
{
    if (currenttxt->currentinuse == 1) {
        if (currenttxt->algochoice != nullptr)
            currenttxt->algochoice->setState();
    }
    currenttxt->currentinuse = 1;
}

bool textsf::unsetState()
{
    bool done = 0;
    if (currenttxt->algochoice != nullptr) {
        done = currenttxt->algochoice->unsetState();
    }
    if (currenttxt->currentinuse == 1 && done != 1) {
        currenttxt->currentinuse = 0;
        return 1;
    } else
        return 0;
}

void textsf::textsfDown()
{
    if (currenttxt->currentinuse == 0) {
        if (currenttxt->next != nullptr) {
            currenttxt->text.setFillColor(sf::Color::White);
            currenttxt = currenttxt->next;
            currenttxt->text.setFillColor(sf::Color::Red);
        } else {
            currenttxt->text.setFillColor(sf::Color::White);
            currenttxt = first;
            currenttxt->text.setFillColor(sf::Color::Red);
        }
    } else {
        currenttxt->algochoice->textsfDown();
    }
}

void textsf::textsfUp()
{
    if (currenttxt->currentinuse == 0) {
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
    } else {
        currenttxt->algochoice->textsfUp();
    }
}

textsf::textsf()
{

}

void textsf::drawall()
{
    if (currenttxt->currentinuse == 0) {
        text.setPosition(pos);
        window->draw(text);
        if (next != nullptr) {
            next->drawall();
        }
    } else {
        currenttxt->algochoice->drawall();
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