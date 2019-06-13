//
//  main.cpp"
//  sfml
//
//  Created by Antonio Toche on 6/12/19.
//  Copyright © 2019 Antonio Toche. All rights reserved.
//

//#include <SFML/Graphics.hpp>
#include <iostream>
#include "Header.h"
#include <vector>
#include <string>
#include <cstdlib>
#include <fstream>
#include <sstream>

//#include <SFML/Window.hpp>

/* 
void setCircle(float x, float y,sf::RenderWindow& window){
        sf::CircleShape nodeDraw;
        nodeDraw.setRadius(20);
        nodeDraw.setFillColor(sf::Color(164, 0, 254));
        nodeDraw.setPosition(x, y);
        window.draw(nodeDraw);
}*/

int main()
{

    //sf::RenderWindow window(sf::VideoMode(1000, 1000), ("Graph Project"));
    std::vector<node> nodes;
    std::vector<float> posx, posy;
    int valueNode1, rows, i;
    std::cout << "sdasd "<< std::endl;
    std::cin >> rows;
    std::string file = "input.txt", line;
    std::fstream f(file);
    float valuex, valuey;
    if (f.is_open())
    {
        std::getline(f, line);
        std::string::size_type cz = 0;
        rows = stoi(line, &cz);

        for (i = 0; i < rows; i++)
        {
            getline(f, line);
            std::stringstream s(line);
            s >> valueNode1 >> valuex >> valuey;
            posx.push_back(valuex);
            posy.push_back(valuey);
            std::cout << valuex << "   " << valuey << std::endl;
        }

        while (getline(f, line))
        {
        }
    }

    std::cout << rows << std::endl;
    for (int i = 0; i < rows; i++)
    {
        nodes.push_back(node(posx.at(i), posy.at(i)));
        std::cout << nodes.at(i)._posX << std::endl;
    }

    /*  while (window.isOpen())
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
      
        float a = nodes.at(2).getX();
        float b = nodes.at(3).getY();
      //  for(int i = 0;i < rows;i++){
        setCircle(a, b, window);
    //}
        
        window.display();
    }*/
    //float a = nodes.at(2).getX();
    //float b = nodes.at(3).getY();
    std::cout << 12 << "  " << 12 << std::endl;

    return 0;
}
