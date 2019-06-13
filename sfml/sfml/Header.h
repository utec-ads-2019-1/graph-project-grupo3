//
//  Header.h
//  sfml
//
//  Created by Antonio Toche on 6/12/19.
//  Copyright © 2019 Antonio Toche. All rights reserved.
//

#ifndef Header_h
#define Header_h
class node{
public:
    float _posX, _posY;
    float edges[2];
    float peso;
    node(float x, float y): _posX(x), _posY(y)
    {}
    
    node(float _peso,float node1,float node2){
        edges[0] = node1;
        edges[1] = node2;
        peso = _peso;
    }
    
    float getX(){return _posX;}
    float getY(){return _posY;}
};

#endif /* Header_h */
