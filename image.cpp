#include "gui.hpp"



sfimage::sfimage(sf::RenderWindow *windows) : background(), Damier()
{
    window = windows;
    if (!backtexture.loadFromFile("dark-grey.jpg"))
        throw EXIT_FAILURE;
    backsprite.setTexture(backtexture, true);
    sf::Texture temp;
    sf::Rect<int> Q;
    Q.left = 80;
    Q.top = 60;
    Q.width = 20;
    Q.height = 20;
    if (!temp.loadFromFile("font.png", Q))
        throw EXIT_FAILURE;
    qfont = temp.copyToImage();
    Q.left = 40;
    Q.top = 20;
    Q.width = 20;
    Q.height = 20;
    for (int i = 0; i < 9; i++)
    {
        if (!temp.loadFromFile("font.png", Q))
            throw EXIT_FAILURE;
        nfont[i] = temp.copyToImage();
        Q.left += 20;
    }
    
    //sf::Sprite sprite(temp, Q);
    
    
}

std::vector<std::vector<int>> *returntable(std::string name)
{
    std::vector<std::vector<int>> *intvector = new std::vector<std::vector<int>>;
    std::ifstream file;
    file.open(name);
    if (file.is_open()) {
        std::string line;
        for (int i = 0; std::getline(file, line); i++) {
            std::stringstream ss(line);
            if (line.length() < 2)
                break;
            std::string s;
            std::vector<int> *temp = new std::vector<int>;
            for (int n = 0; std::getline(ss, s, '|'); n++) {
                temp->push_back(atoi(s.c_str()));
            }
            intvector->push_back(*temp);
            
        }
        file.close();
    }
    std::cout << intvector->at(0).at(2) << std::endl;
    return intvector;
}

int sfimage::createcorrect(std::string name)
{
    if (name == "20-Queens") {
            createdam(8);
            damiertexture.loadFromImage(Damier);
            damiersprite.setTexture(damiertexture);
            damiersprite.setPosition(IMGSIZE, 0);
            return 1;
    } else if (name == "8-puzzle") {
        createpuz(8);
        initialstate = returntable("init8");
        //finalstate = returntable("end8");
        //addpuzzle(*finalstate, *initialstate);
        puzunsoltexture.loadFromImage(puzunsol);
        puzunsolsprite.setTexture(puzunsoltexture);
        puzunsolsprite.setPosition(IMGSIZE, 0);
        puzsoltexture.loadFromImage(puzsol);
        puzsolsprite.setTexture(puzsoltexture);
        puzsolsprite.setPosition(IMGSIZE + 5 +(IMGSIZE / 2), 0);
        return 8;
    } else if (name == "24-puzzle") {
        createpuz(24);
        initialstate = returntable("init24");
        //finalstate = returntable("end24");
        //addpuzzle(*finalstate, *initialstate);
        puzunsoltexture.loadFromImage(puzunsol);
        puzunsolsprite.setTexture(puzunsoltexture);
        puzunsolsprite.setPosition(IMGSIZE, 0);
        puzsoltexture.loadFromImage(puzsol);
        puzsolsprite.setTexture(puzsoltexture);
        puzsolsprite.setPosition(IMGSIZE + 5 +(IMGSIZE / 2), 0);
        return 24;
    }
    return 4;
}

void sfimage::drawall(textsf *textbar)
{
    window->draw(backsprite);
    if (textbar->currenttxt->currentinuse == 1) {
        if (textbar->currenttxt->textcontent == "20-Queens") {
            window->draw(damiersprite);
        } else if (textbar->currenttxt->textcontent == "8-puzzle" || textbar->currenttxt->textcontent == "24-puzzle") {
            window->draw(puzunsolsprite);
            window->draw(puzsolsprite);
        }
    }
}

void sfimage::addqueen()
{
    
}

void sfimage::addpuzzle(std::vector<std::vector<int>> &grid, std::vector<std::vector<int>> &grid2)
{
    int temp = puzsize;
    createpuz(sqrt(puzsize + 1));
    puzsize = temp;
    int tempwidth = puzspaces;
    int tempheight = puzspaces;
    for (size_t i = 0; i < (sqrt(puzsize + 1)); i++)
    {
        for (size_t n = 0; n < (sqrt(puzsize + 1)); n++)
        {
            if (grid.at(i).at(n) != 0) {
                puzsol.copy(nfont[(grid2.at(i).at(n) - 1)], tempwidth, tempheight);
            }
            tempwidth += puzsquare + puzspaces;
        }
        tempwidth = puzspaces;
        tempheight += puzsquare + puzspaces;
    }
    for (size_t i = 0; i < (sqrt(puzsize + 1)); i++)
    {
        for (size_t n = 0; n < (sqrt(puzsize + 1)); n++)
        {
            if (grid2.at(i).at(n) != 0) {
                puzunsol.copy(nfont[grid2.at(i).at(n) - 1], tempwidth, tempheight);
            }
            tempwidth += puzsquare + puzspaces;
        }
        tempwidth = puzspaces;
        tempheight += puzsquare + puzspaces;
    }
}

void sfimage::createpuz(int size)
{
    int temp = size;
    size = sqrt(size + 1);
    puzsol.create(IMGSIZE /2, IMGSIZE/2, sf::Color::Black);
    puzunsol.create(IMGSIZE /2, IMGSIZE/2, sf::Color::Black);
    int spaces = 5 * size;
    int squaresize = ((IMGSIZE /2) - spaces) / size;
    spaces = spaces / size;
    puzspaces = spaces;
    puzsquare = squaresize;
    puzsize = size;
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
            if (n%2 == 0) {
                puzsol.copy(squareWhite, tempwidth, tempheight);
                puzunsol.copy(squareWhite, tempwidth, tempheight);
            } else {
                puzsol.copy(squareRed, tempwidth, tempheight);
                puzunsol.copy(squareRed, tempwidth, tempheight);
            }
            tempwidth += squaresize + spaces;
        }
        tempwidth = spaces;
        tempheight += squaresize + spaces;
    }
    size = temp;
}

void sfimage::createdam(int size)
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
    damierqueen = Damier;
}


sfimage::~sfimage()
{
}