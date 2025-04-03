#include "Crawler.h"
#include <cstdlib>
#include <ctime>
#include <sstream>

Crawler::Crawler(int id, int x, int y, Direction direction, int size) 
    : id(id), direction(direction), size(size), alive(true) {
    position.x = x;
    position.y = y;
    path.push_back(position);
}

int Crawler::getId() const { return id; }
Position Crawler::getPosition() const { return position; }
Direction Crawler::getDirection() const { return direction; }
int Crawler::getSize() const { return size; }
bool Crawler::isAlive() const { return alive; }
std::list<Position> Crawler::getPath() const { return path; }

void Crawler::setPosition(Position newPosition) { 
    position = newPosition; 
    path.push_back(position);
}

void Crawler::setDirection(Direction newDirection) { direction = newDirection; }
void Crawler::setSize(int newSize) { size = newSize; }
void Crawler::setAlive(bool status) { alive = status; }

bool Crawler::isWayBlocked() const {
    switch (direction) {
        case Direction::NORTH: return position.y == 0;
        case Direction::EAST: return position.x == 9;
        case Direction::SOUTH: return position.y == 9;
        case Direction::WEST: return position.x == 0;
        default: return false;
    }
}

void Crawler::move() {
    if (!alive) return;

    Position newPosition = position;
    
    switch (direction) {
        case Direction::NORTH: newPosition.y--; break;
        case Direction::EAST: newPosition.x++; break;
        case Direction::SOUTH: newPosition.y++; break;
        case Direction::WEST: newPosition.x--; break;
    }
    

    if (newPosition.x >= 0 && newPosition.x < 10 && 
        newPosition.y >= 0 && newPosition.y < 10) {
        position = newPosition;
        path.push_back(position);
    } else {

        Direction originalDirection = direction;
        do {
            direction = static_cast<Direction>(rand() % 4 + 1);
        } while (direction == originalDirection || isWayBlocked());
        
        
        move();
    }
}

std::string Crawler::getDirectionAsString() const {
    switch (direction) {
        case Direction::NORTH: return "North";
        case Direction::EAST: return "East";
        case Direction::SOUTH: return "South";
        case Direction::WEST: return "West";
        default: return "Unknown";
    }
}

std::string Crawler::toString() const {
    std::ostringstream oss;
    oss << id << " Crawler (" << position.x << "," << position.y << ") " 
        << size << " " << getDirectionAsString() << " " 
        << (alive ? "Alive" : "Dead");
    return oss.str();
}