#include "main.hpp"

bool eventloop(sf::RenderWindow &window, textsf &textbar)
{
    sf::Event event;

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
            textbar.textsfDown();
            } else if (event.key.code == sf::Keyboard::Up)
            {
                textbar.textsfUp();
            } else if (event.key.code == sf::Keyboard::Enter)
            {
                textbar.currenttxt->activity(&window);
            }

        }
    }
    return 1;
}

void activity1(sf::RenderWindow *window)
{
    printf("yolo");
}
void activity2(sf::RenderWindow *window)
{
    printf("wolo");
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(SCRWIDTH, SCRHEIGHT), "My alcohol ai");
    sfimage images(&window);
    images.createdams(8);
    textsf textbar("8-puzzle", &window, &textbar);
    textbar.activity = &activity1;
    textbar.newfoltext("24-puzzle", &activity2);
    textbar.newfoltext("20-Queens", &activity1);
    textbar.newfoltext("1000000-Queens", &activity1);

    while (1) {
        window.clear();
        if (eventloop(window, textbar) == 0) {
            return 0;
        }
        images.drawall();
        textbar.drawall();
        window.display();
    }
    // Load an image file from a file
    sf::Image background;
    if (!background.loadFromFile("background.jpg"))
        return -1;
    // Create a 20x20 image filled with black color
    sf::Image image;
    image.create(20, 20, sf::Color::Black);
    // Copy image1 on image2 at position (10, 10)
    image.copy(background, 10, 10);
    // Make the top-left pixel transparent
    sf::Color color = image.getPixel(0, 0);
    color.a = 0;
    image.setPixel(0, 0, color);
    // Save the image to a file
    if (!image.saveToFile("result.png"))
        return -1;
    return 0;
}