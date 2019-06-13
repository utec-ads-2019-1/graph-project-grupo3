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
    
    node(float x, float y): _posX(x), _posY(y)
    {}
    
    float getX(){return _posX;}
    float getY(){return _posY;}
};

#endif /* Header_h */
