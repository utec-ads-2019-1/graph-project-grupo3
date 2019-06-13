//#include <SFML/Graphics.hpp>
//#include <SFML/Graphics.hpp>
#include <iostream>
//#include "Header.h"
#include <vector>
#include <string>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <SFML/Window.hpp>
/* 
void setCircle(float x, float y, sf::RenderWindow &window)
{
    sf::CircleShape nodeDraw;
    nodeDraw.setRadius(20);
    nodeDraw.setFillColor(sf::Color(164, 0, 254));
    nodeDraw.setPosition(x, y);
    window.draw(nodeDraw);
}*/

int main()
{

    // sf::RenderWindow window(sf::VideoMode(1000, 1000), ("Graph Project"));
    // std::vector<node> nodes;

    int valueNode1, rows, i;
    std::string file = "input.txt", line;
    std::fstream f(file);
    float valuex, valuey;

    std::getline(f, line);
    std::string::size_type cz = 0;

    rows = stoi(line, &cz);
    float *arrayX = new float[rows];
    float *arrayY = new float[rows];

    //int arraylist[3] = {600, 800, 300};
    if (f.is_open())
    {
        //std::getline(f, line);
        //std::string::size_type cz = 0;

        //rows = stoi(line, &cz);
        for (i = 0; i < rows; i++)
        {
            getline(f, line);
            std::stringstream s(line);
            s >> valueNode1 >> valuex >> valuey;
            arrayX[i] = valuex;
            arrayY[i] = valuey;
            //posx.push_back(valuex);
            //posy.push_back(valuey);
            std::cout << valuex << " " << valuey << std::endl;
            std::cout << arrayX[i] + arrayY[i] << std::endl;
        }

        while (getline(f, line))
        {
        }
    }

    /* for (int i = 0; i < rows; i++)
    {
        nodes.push_back(node(posx.at(i), posy.at(i)));
    }

      while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }
        window.clear();

        for (int i = 0; i < rows; i++)
        {
            setCircle(arrayX[i], arrayY[i], window);
        }

        window.display();
    }
*/
    return 0;
}
