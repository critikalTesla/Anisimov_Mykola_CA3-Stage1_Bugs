#ifndef CRAWLER_H
#define CRAWLER_H

#include "Position.h"
#include <list>
#include <string>

enum class Direction { NORTH = 1, EAST = 2, SOUTH = 3, WEST = 4 };

class Crawler {
private:
    int id;
    Position position;
    Direction direction;
    int size;
    bool alive;
    std::list<Position> path;

public:
    Crawler(int id, int x, int y, Direction direction, int size);
    

    int getId() const;
    Position getPosition() const;
    Direction getDirection() const;
    int getSize() const;
    bool isAlive() const;
    std::list<Position> getPath() const;
    

    void setPosition(Position newPosition);
    void setDirection(Direction newDirection);
    void setSize(int newSize);
    void setAlive(bool status);
    

    void move();
    bool isWayBlocked() const;
    

    std::string getDirectionAsString() const;
    std::string toString() const;
};

#endif