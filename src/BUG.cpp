#include "Bug.h"
#include <sstream>

Bug::Bug(int id, int x, int y, Direction direction, int size) 
    : id(id), direction(direction), size(size), alive(true) {
    position.x = x;
    position.y = y;
    path.push_back(position);
}

void Bug::setPosition(Position newPosition) { 
    position = newPosition; 
    path.push_back(position);
}

void Bug::setDirection(Direction newDirection) { direction = newDirection; }
void Bug::setSize(int newSize) { size = newSize; }
void Bug::setAlive(bool status) { alive = status; }

bool Bug::isWayBlocked() const {
    switch (direction) {
        case Direction::NORTH: return position.y == 0;
        case Direction::EAST: return position.x == 9;
        case Direction::SOUTH: return position.y == 9;
        case Direction::WEST: return position.x == 0;
        default: return false;
    }
}

std::string Bug::getDirectionAsString() const {
    switch (direction) {
        case Direction::NORTH: return "North";
        case Direction::EAST: return "East";
        case Direction::SOUTH: return "South";
        case Direction::WEST: return "West";
        default: return "Unknown";
    }
}

std::string Bug::toString() const {
    std::ostringstream oss;
    oss << id << " " << getType() << " (" << position.x << "," << position.y << ") " 
        << size << " " << getDirectionAsString() << " " 
        << (alive ? "Alive" : "Dead");
    return oss.str();
}