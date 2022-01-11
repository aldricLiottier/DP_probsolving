#include "main.hpp"
#include <iostream>
#include <fstream>

Algorithm::STRAT returnstrat(textsf *textbar)
{
    if (textbar->currenttxt->algochoice->currenttxt->textcontent == "uninformed search") {
        return Algorithm::UNIFORMED;
    } else if (textbar->currenttxt->algochoice->currenttxt->textcontent == "local search") {
        return Algorithm::LOCAL;
    } else
        return Algorithm::INFORMED;
}

int returnstateur(textsf *textbar)
{
    if (textbar->currenttxt->algochoice->currenttxt->algochoice->currenttxt->textcontent == "one step") {
        return 1;
    } else if (textbar->currenttxt->algochoice->currenttxt->algochoice->currenttxt->textcontent == "full solve") {
        return 2;
    } else
        return 3;
}

bool eventloop(sf::RenderWindow &window, textsf *textbar, sfimage *images, senting *sender)
{
    sf::Event event;
    int temp = 0;
    int value = 1;

// while there are pending events...
    while (window.pollEvent(event))
    {
        // check the type of the event...
        if (event.type == sf::Event::Closed) {
            window.close();
            return 0;
        } else if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Down) {
            textbar->textsfDown();
            } else if (event.key.code == sf::Keyboard::Up)
            {
                textbar->textsfUp();
            } else if (event.key.code == sf::Keyboard::Enter) {
                if (textbar->currenttxt->currentinuse == 0) {
                    temp = images->createcorrect(textbar->currenttxt->textcontent);
                } else if (textbar->currenttxt->algochoice->currenttxt->currentinuse == 0) {
                    if (textbar->currenttxt->textcontent == "8-puzzle") {
                        textbar->currenttxt->Puzzlealgo = new Algorithm(8, returnstrat(textbar));
                        value = 2;
                    } else if (textbar->currenttxt->textcontent == "24-puzzle") {
                        textbar->currenttxt->Puzzlealgo = new Algorithm(24, returnstrat(textbar));
                        value = 2;
                    } else if (textbar->currenttxt->textcontent == "20-Queens") {
                        textbar->currenttxt->Puzzlealgo = new NQueens(20);
                        value = 2;
                    } else if (textbar->currenttxt->textcontent == "1000000-Queens") {
                        textbar->currenttxt->Puzzlealgo = new NQueens(1000000);
                        value = 2;
                    }
                } //else if (textbar->currenttxt->algochoice->currenttxt->algochoice->currenttxt->currentinuse == 0) {
                    //sender->action = returnstateur(textbar);
                    //std::thread worker(textbar->currenttxt->Puzzlealgo->threadAlgo(images->initialstate, &sender->action, &sender->strat));
                    //sender->worker_thread = std::move();
                //}
                textbar->setState();
            } else if (event.key.code == sf::Keyboard::Escape) {
                textbar->unsetState();
            }
        }
    }
    return value;
}


int main()
{
    sf::RenderWindow window(sf::VideoMode(SCRWIDTH, SCRHEIGHT), "My alcohol ai");
    sfimage images(&window);
    senting *sender;
    textsf *textbar = new textsf("8-puzzle", &window, nullptr);
    int state = 1;
    textbar->newfoltext("24-puzzle", nullptr);
    textbar->newfoltext("20-Queens", nullptr);
    textbar->newfoltext("1000000-Queens", nullptr);
    while (1) {
        window.clear();
        images.drawall(textbar);
        textbar->drawall();
        window.display();
        state = eventloop(window, textbar, &images, sender);
        if (state == 0) {
            return 0;
        }
    }
    return 0;
}