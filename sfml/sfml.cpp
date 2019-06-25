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
#include <time.h>
#include <vector>
#include <string>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <cmath>
#include "button.h"
#include <SFML/Window.hpp>

float rectEquationX(float x1, float x2)
{
    float posx = (x2 + x1) / 2;
    return posx;
}

float rectEquationY(float y1, float y2)
{
    float posy = (y2 + y1) / 2;
    return posy;
}
void Textos(std::string texto, sf::Font &font, sf::RenderWindow &window, int posX, int posY)
{
    sf::Text textTo;
    textTo.setString(texto);
    textTo.setCharacterSize(30);
    textTo.setFillColor(sf::Color::Black);
    textTo.setFont(font);
    textTo.setPosition(posX, posY);
    window.draw(textTo);
}
void setDirection(float x1, float y1, float x2, float y2, sf::RenderWindow &window)
{
    sf::CircleShape triangle;
    triangle.setRadius(10);
    triangle.setPointCount(3);
    float newX1 = rectEquationX(x1, x2);
    float newY1 = rectEquationY(y1, y2);
    float newPosY1 = (y2 + newY1) / 2;
    float newPosX1 = (x2 + newX1) / 2;
    /* float pendient = (y2 - newPosY1) / (x2 - newPosX1);
    pendient = atan(pendient);
    sf::Rect<float> size = triangle.getGlobalBounds();
    triangle.setOrigin(sf::Vector2f((size.width/2)/5,(size.height/2)/5));
    std::cout << "Arco tang: " << pendient << std::endl;
    triangle.setRotation(pendient*10);*/
    triangle.setPosition(newPosX1, newPosY1);
    triangle.setFillColor(sf::Color(255, 0, 0));
    triangle.setOutlineColor(sf::Color(0, 0, 255));
    window.draw(triangle);
}
void setLines(float x1, float y1, float x2, float y2, sf::RenderWindow &window, int peso)
{
    setDirection(x1, y1, x2, y2, window);
    std::string weight = std::to_string(peso);
    sf::Text text;
    sf::Font font;
    if (!font.loadFromFile("Quicksand-Bold.ttf"))
    {
        std::cout << "error loading font" << std::endl;
    }
    text.setString(weight);
    text.setFillColor(sf::Color(63, 63, 63));
    int posX = rectEquationX(x1, x2);
    int posY = rectEquationY(y1, y2);
    text.setPosition(posX, posY);
    text.setFont(font);
    text.setCharacterSize(20);
    sf::Vertex line[] = {
        sf::Vertex(sf::Vector2f(x1 + 10, y1 + 10)),
        sf::Vertex(sf::Vector2f(x2 + 10, y2 + 10))};
    window.draw(line, 10, sf::Lines);
    window.draw(text);
}

void setCircle(float x, float y, sf::RenderWindow &window, char i)
{
    sf::CircleShape nodeDraw(360);
    sf::Text text;
    sf::Font font;
    if (!font.loadFromFile("Quicksand-Bold.ttf"))
    {
        std::cout << "error loading font" << std::endl;
    }
    text.setString(i);
    text.setFillColor(sf::Color(0, 255, 0));
    text.setPosition(x + 9, y + 9);
    text.setFont(font);
    text.setCharacterSize(20);
    nodeDraw.setRadius(20);
    nodeDraw.setFillColor(sf::Color(255, 255, 255));
    nodeDraw.setOutlineColor(sf::Color(0, 178, 209));
    nodeDraw.setOutlineThickness(5);
    nodeDraw.setPosition(x, y);
    window.draw(nodeDraw);
    window.draw(text);
}

void GraphState()
{
    sf::RenderWindow window(sf::VideoMode(1200, 1200), ("Graph"));
    sf::Font font;
    font.loadFromFile("Quicksand-Bold.ttf");
    std::vector<node> nodes;
    std::vector<edge> edges;
    std::vector<float> posx, posy,pesos,conexion1,conexion2;
    int valueNode1, valuePeso, rows, i;
    std::string file = "../input.txt", line;
    std::fstream f(file);
    float valuex, valuey, valueX, valueY;
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
        }
        getline(f, line);
        while (getline(f, line))
        {
            std::stringstream j(line);
            j >> valueX >> valueY >> valuePeso;
            conexion1.push_back(valueX);
            conexion2.push_back(valueY);
            pesos.push_back(valuePeso);
        }
    }
    for (int i = 0; i < pesos.size(); i++)
    {
        float pos1 = conexion1.at(i) - 1;
        float pos2 = conexion2.at(i) - 1;
        float x1 = posx.at(pos1);
        float y1 = posy.at(pos1);
        float x2 = posx.at(pos2);
        float y2 = posy.at(pos2);
        edges.push_back(edge(x1, y1, x2, y2));
    }

    for (int i = 0; i < rows; i++)
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
        window.clear(sf::Color(0, 202, 165));

        for (int i = 0; i < rows; i++)
        {
            char number = i + 97;
            setCircle(nodes.at(i)._posX, nodes.at(i)._posY, window, number);
        }

        for (int i = 0; i < pesos.size(); i++)
        {
            int peso = pesos.at(i);
            setLines(edges.at(i)._posX1, edges.at(i)._posY1, edges.at(i)._posX2, edges.at(i)._posY2, window, peso);
        }
        Textos("Grafos grupo 3", font, window, 500, 50);
        window.display();
    }
}


void Dijkstra()
{
    sf::Font font;
    font.loadFromFile("Quicksand-Bold.ttf");
    sf::RenderWindow window(sf::VideoMode(1200, 1200), ("Dijkstra"));
    std::vector<node> nodes;
    std::vector<edge> edges;
    std::vector<float> posx, posy,pesos,conexion1,conexion2;
    int valueNode1, valuePeso, rows, i;
    std::string file = "../input.txt", line;
    std::fstream f(file);
    float valuex, valuey, valueX, valueY;
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
        }
        while (getline(f, line))
        {
        }
    }
    std::string filebfs = "../dijkstra.txt", bfsline;
    std::fstream bfsf(filebfs);
    if(bfsf.is_open()){
        while(getline(bfsf,bfsline)){
         std::stringstream j(bfsline);
            j >> valueX >> valueY >> valuePeso;
            conexion1.push_back(valueX);
            conexion2.push_back(valueY);
            pesos.push_back(valuePeso);
        }
    }
    for (int i = 0; i < pesos.size(); i++)
    {
        float pos1 = conexion1.at(i) - 1;
        float pos2 = conexion2.at(i) - 1;
        float x1 = posx.at(pos1);
        float y1 = posy.at(pos1);
        float x2 = posx.at(pos2);
        float y2 = posy.at(pos2);
        edges.push_back(edge(x1, y1, x2, y2));
    }

    for (int i = 0; i < rows; i++)
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
        window.clear(sf::Color(0, 202, 165));

        for (int i = 0; i < rows; i++)
        {
            char number = i + 97;
            setCircle(nodes.at(i)._posX, nodes.at(i)._posY, window, number);
        }

        for (int i = 0; i < pesos.size(); i++)
        {
            int peso = pesos.at(i);
            setLines(edges.at(i)._posX1, edges.at(i)._posY1, edges.at(i)._posX2, edges.at(i)._posY2, window, peso);
        }
        Textos("Dijkstra", font, window, 500, 50);
        window.display();
    }
}



void A()
{
    sf::Font font;
    font.loadFromFile("Quicksand-Bold.ttf");
    sf::RenderWindow window(sf::VideoMode(1200, 1200), ("A*"));
    std::vector<node> nodes;
    std::vector<edge> edges;
    std::vector<float> posx, posy,pesos,conexion1,conexion2;
    int valueNode1, valuePeso, rows, i;
    std::string file = "../input.txt", line;
    std::fstream f(file);
    float valuex, valuey, valueX, valueY;
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
        }
        while (getline(f, line))
        {
        }
    }
    std::string filebfs = "../A.txt", bfsline;
    std::fstream bfsf(filebfs);
    if(bfsf.is_open()){
        while(getline(bfsf,bfsline)){
         std::stringstream j(bfsline);
            j >> valueX >> valueY >> valuePeso;
            conexion1.push_back(valueX);
            conexion2.push_back(valueY);
            pesos.push_back(valuePeso);
        }
    }
    for (int i = 0; i < pesos.size(); i++)
    {
        float pos1 = conexion1.at(i) - 1;
        float pos2 = conexion2.at(i) - 1;
        float x1 = posx.at(pos1);
        float y1 = posy.at(pos1);
        float x2 = posx.at(pos2);
        float y2 = posy.at(pos2);
        edges.push_back(edge(x1, y1, x2, y2));
    }

    for (int i = 0; i < rows; i++)
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
        window.clear(sf::Color(0, 202, 165));

        for (int i = 0; i < rows; i++)
        {
            char number = i + 97;
            setCircle(nodes.at(i)._posX, nodes.at(i)._posY, window, number);
        }

        for (int i = 0; i < pesos.size(); i++)
        {
            int peso = pesos.at(i);
            setLines(edges.at(i)._posX1, edges.at(i)._posY1, edges.at(i)._posX2, edges.at(i)._posY2, window, peso);
        }
        Textos("A*", font, window, 500, 50);
        window.display();
    }
}




void floydWarshall()
{
    sf::Font font;
    font.loadFromFile("Quicksand-Bold.ttf");
    sf::RenderWindow window(sf::VideoMode(1200, 1200), ("Floyd Warshall"));
    std::vector<node> nodes;
    std::vector<edge> edges;
    std::vector<float> posx, posy,pesos,conexion1,conexion2;
    int valueNode1, valuePeso, rows, i;
    std::string file = "../input.txt", line;
    std::fstream f(file);
    float valuex, valuey, valueX, valueY;
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
        }
        while (getline(f, line))
        {
        }
    }
    std::string filebfs = "../floydWarshall.txt", bfsline;
    std::fstream bfsf(filebfs);
    if(bfsf.is_open()){
        while(getline(bfsf,bfsline)){
         std::stringstream j(bfsline);
            j >> valueX >> valueY >> valuePeso;
            conexion1.push_back(valueX);
            conexion2.push_back(valueY);
            pesos.push_back(valuePeso);
        }
    }
    for (int i = 0; i < pesos.size(); i++)
    {
        float pos1 = conexion1.at(i) - 1;
        float pos2 = conexion2.at(i) - 1;
        float x1 = posx.at(pos1);
        float y1 = posy.at(pos1);
        float x2 = posx.at(pos2);
        float y2 = posy.at(pos2);
        edges.push_back(edge(x1, y1, x2, y2));
    }

    for (int i = 0; i < rows; i++)
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
        window.clear(sf::Color(0, 202, 165));

        for (int i = 0; i < rows; i++)
        {
            char number = i + 97;
            setCircle(nodes.at(i)._posX, nodes.at(i)._posY, window, number);
        }

        for (int i = 0; i < pesos.size(); i++)
        {
            int peso = pesos.at(i);
            setLines(edges.at(i)._posX1, edges.at(i)._posY1, edges.at(i)._posX2, edges.at(i)._posY2, window, peso);
        }
        Textos("Floyd Warshall", font, window, 500, 50);
        window.display();
    }
}






void bellmanFord()
{
    sf::Font font;
    font.loadFromFile("Quicksand-Bold.ttf");
    sf::RenderWindow window(sf::VideoMode(1200, 1200), ("Bellman Ford"));
    std::vector<node> nodes;
    std::vector<edge> edges;
    std::vector<float> posx, posy,pesos,conexion1,conexion2;
    int valueNode1, valuePeso, rows, i;
    std::string file = "../input.txt", line;
    std::fstream f(file);
    float valuex, valuey, valueX, valueY;
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
        }
        while (getline(f, line))
        {
        }
    }
    std::string filebfs = "../bellmanford.txt", bfsline;
    std::fstream bfsf(filebfs);
    if(bfsf.is_open()){
        while(getline(bfsf,bfsline)){
         std::stringstream j(bfsline);
            j >> valueX >> valueY >> valuePeso;
            conexion1.push_back(valueX);
            conexion2.push_back(valueY);
            pesos.push_back(valuePeso);
        }
    }
    for (int i = 0; i < pesos.size(); i++)
    {
        float pos1 = conexion1.at(i) - 1;
        float pos2 = conexion2.at(i) - 1;
        float x1 = posx.at(pos1);
        float y1 = posy.at(pos1);
        float x2 = posx.at(pos2);
        float y2 = posy.at(pos2);
        edges.push_back(edge(x1, y1, x2, y2));
    }

    for (int i = 0; i < rows; i++)
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
        window.clear(sf::Color(0, 202, 165));

        for (int i = 0; i < rows; i++)
        {
            char number = i + 97;
            setCircle(nodes.at(i)._posX, nodes.at(i)._posY, window, number);
        }

        for (int i = 0; i < pesos.size(); i++)
        {
            int peso = pesos.at(i);
            setLines(edges.at(i)._posX1, edges.at(i)._posY1, edges.at(i)._posX2, edges.at(i)._posY2, window, peso);
        }
        Textos("Bellman Ford", font, window, 500, 50);
        Textos("Distancia desde Source", font, window, 500, 150);
        window.display();
    }
}
int main()
{
    sf::RenderWindow window(sf::VideoMode(1200, 1200), ("Graph"));
    sf::Font font;
    font.loadFromFile("Quicksand-Bold.ttf");
    Button button1("Graph", {200, 50}, 20, sf::Color(157, 227, 158), sf::Color::Black);
    button1.setPosition({500, 300});
    button1.setFont(font);

    Button button2("Dijkstra", {200, 50}, 20, sf::Color(157, 227, 158), sf::Color::Black);
    button2.setPosition({500, 400});
    button2.setFont(font);

    Button button3("    A*", {200, 50}, 20, sf::Color(157, 227, 158), sf::Color::Black);
    button3.setPosition({500, 500});
    button3.setFont(font);

    Button button4("Floyd-Warshall", {200, 50}, 13, sf::Color(157, 227, 158), sf::Color::Black);
    button4.setPosition({500, 600});
    button4.setFont(font);

    Button button5("Bellman-Ford", {200, 50}, 15, sf::Color(157, 227, 158), sf::Color::Black);
    button5.setPosition({500, 700});
    button5.setFont(font);
    while (window.isOpen())
    {
        sf::Event Event;
        while (window.pollEvent(Event))
        {
            switch (Event.type)
            {
            case sf::Event::Closed:
                window.close();
            case sf::Event::MouseMoved:
                if (button1.isMouseOver(window))
                {
                    button1.setBackColor(sf::Color(117, 158, 129));
                }
                else
                {
                    button1.setBackColor(sf::Color(157, 227, 158));
                }
                if (button2.isMouseOver(window))
                {
                    button2.setBackColor(sf::Color(117, 158, 129));
                }
                else
                {
                    button2.setBackColor(sf::Color(157, 227, 158));
                }
                if (button3.isMouseOver(window))
                {
                    button3.setBackColor(sf::Color(117, 158, 129));
                }
                else
                {
                    button3.setBackColor(sf::Color(157, 227, 158));
                }
                if (button4.isMouseOver(window))
                {
                    button4.setBackColor(sf::Color(117, 158, 129));
                }
                else
                {
                    button4.setBackColor(sf::Color(157, 227, 158));
                }
                if (button5.isMouseOver(window))
                {
                    button5.setBackColor(sf::Color(117, 158, 129));
                }
                else
                {
                    button5.setBackColor(sf::Color(157, 227, 158));
                }
                break;
            case sf::Event::MouseButtonPressed:
                if (button1.isMouseOver(window))
                {
                    GraphState();
                }
                if (button2.isMouseOver(window))
                {
                    Dijkstra();
                }
                if (button3.isMouseOver(window))
                {
                    A();
                }
                if (button4.isMouseOver(window))
                {
                    floydWarshall();
                }
                if (button5.isMouseOver(window))
                {
                    bellmanFord();
                }
            default:
                break;
            }
        }
        window.clear(sf::Color(0, 202, 165));
        Textos("Grafos Grupo 3", font, window, 500, 50);
        Textos("Autores: ", font, window, 300, 100);
        Textos("*", font, window, 300, 150);
        Textos("Jhans Ubaldo", font, window, 340, 140);
        Textos("*", font, window, 300, 190);
        Textos("Giordano Alvitez", font, window, 340, 180);
        Textos("*", font, window, 300, 230);
        Textos("Antonio Toche", font, window, 340, 220);
        button1.drawTo(window);
        button2.drawTo(window);
        button3.drawTo(window);
        button4.drawTo(window);
        button5.drawTo(window);
        window.display();
    }
    return 0;
}
