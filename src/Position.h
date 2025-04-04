#ifndef POSITION_H
#define POSITION_H

#include <iostream>

struct Position {
    int x;  //x location
    int y;  //y location
    
    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }
    
    bool operator!=(const Position& other) const {
        return !(*this == other);
    }
};

#endif