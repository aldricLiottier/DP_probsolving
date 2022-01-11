#pragma once
#include "gui.hpp"
#define SCRWIDTH sf::VideoMode::getDesktopMode().width / 1.5
#define SCRHEIGHT sf::VideoMode::getDesktopMode().height / 1.5
#include <pthread.h>

class senting
{
private:
    /* data */
public:
    senting(/* args */);
    int action;
    std::thread worker_thread;
    int strat = 0;
    ~senting();
};

senting::senting(/* args */)
{
}

senting::~senting()
{
}
