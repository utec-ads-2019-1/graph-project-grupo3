//
//  main.cpp"
//  sfml
//
//  Created by Antonio Toche on 6/12/19.
//  Copyright © 2019 Antonio Toche. All rights reserved.
//

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Header.h"

#include <vector>
#include <string>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <SFML/Window.hpp>


void setLines(float x1,float y1,float x2,float y2,sf::RenderWindow& window){
    
    sf::Vertex line[]={
        sf::Vertex(sf::Vector2f(x1,y1)),
        sf::Vertex(sf::Vector2f(x2,y2))
    };
    
    window.draw(line,10,sf::Lines);
}

void setCircle(float x, float y,sf::RenderWindow& window,char i){
        sf::CircleShape nodeDraw(360);
        sf::Text text;
        sf::Font font;
        if(!font.loadFromFile("Quicksand-Bold.ttf")){std::cout << "error loading font"<< std::endl;}
        text.setString(i);
        text.setFillColor(sf::Color(0,255,0));
        text.setPosition(x+10, y+10);
        text.setFont(font);
        text.setCharacterSize(20);
        nodeDraw.setRadius(20);
        nodeDraw.setFillColor(sf::Color(255, 255, 255));
        nodeDraw.setOutlineColor(sf::Color(0,178,209));
        nodeDraw.setOutlineThickness(5);
        nodeDraw.setPosition(x, y);
        window.draw(nodeDraw);
        window.draw(text);
}

int main()
{

    sf::RenderWindow window(sf::VideoMode(1200, 1200), ("Graph Project"));
    std::vector<node> nodes;
    std::vector<float> posx, posy;
    std::vector<float> pesos;
    std::vector<int> conexion1;
    std::vector<int> conexion2;
    int valueNode1, rows, i;
    std::string file = "../../input.txt", line;
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
            s >> valueNode1 >> valuex >> valuex;
            posx.push_back(valuex);
            posy.push_back(valuey);
        }
        getline(f,line);
        
        while(getline(f,line)){
            std::stringstream s(line);
            s>>valueNode1>>valuex>>valuey;
            pesos.push_back(valueNode1);
            conexion1.push_back(valuex);
            conexion2.push_back(valuey);
        }
    }
    for (int i = 0; i < rows; i++)
    {
        nodes.push_back(node(posx.at(i), posy.at(i)));
    }
    for(int i = 0; i < conexion1.size();i++){
        
        
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
        window.clear(sf::Color(0,202,165));
      
        
      for(int i = 0;i < rows;i++){
        char number = i + 97;
        setCircle(nodes.at(i)._posX, nodes.at(i)._posY, window,number);
        
          
        
    }
        setLines(100, 200, 300, 200, window);
        window.display();
    }

    return 0;
}
