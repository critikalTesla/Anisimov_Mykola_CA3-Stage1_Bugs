#ifndef BUG_H
#define BUG_H

#include "Position.h"
#include <list>
#include <string>

enum class Direction { NORTH = 1, EAST = 2, SOUTH = 3, WEST = 4 };

class Bug {
protected:
    int id;
    Position position;
    Direction direction;
    int size;
    bool alive;
    std::list<Position> path;

public:
    Bug(int id, int x, int y, Direction direction, int size);
    virtual ~Bug() = default;

    int getId() const { return id; }
    Position getPosition() const { return position; }
    Direction getDirection() const { return direction; }
    int getSize() const { return size; }
    bool isAlive() const { return alive; }
    std::list<Position> getPath() const { return path; }

    void setPosition(Position newPosition);
    void setDirection(Direction newDirection);
    void setSize(int newSize);
    void setAlive(bool status);


    virtual void move() = 0; 
    virtual bool isWayBlocked() const;
    virtual std::string getType() const = 0;
    virtual std::string toString() const;


    std::string getDirectionAsString() const;
};

#endif