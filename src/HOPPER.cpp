#include "Hopper.h"
#include <cstdlib>
#include <ctime>
#include <sstream>

Hopper::Hopper(int id, int x, int y, Direction direction, int size, int hopLength) 
    : Bug(id, x, y, direction, size), hopLength(hopLength) {
    srand(time(0)); // Initialize random seed
}

void Hopper::move() {
    if (!alive) return;

    Position newPosition = position;
    int steps = hopLength;

    switch (direction) {
        case Direction::NORTH: newPosition.y -= steps; break;
        case Direction::EAST: newPosition.x += steps; break;
        case Direction::SOUTH: newPosition.y += steps; break;
        case Direction::WEST: newPosition.x -= steps; break;
    }

    if (newPosition.x >= 0 && newPosition.x < 10 && newPosition.y >= 0 && newPosition.y < 10) {
        setPosition(newPosition);
    } else {
        // Adjust position to hit the edge
        newPosition = position;
        switch (direction) {
            case Direction::NORTH: newPosition.y = std::max(0, position.y - steps); break;
            case Direction::EAST: newPosition.x = std::min(9, position.x + steps); break;
            case Direction::SOUTH: newPosition.y = std::min(9, position.y + steps); break;
            case Direction::WEST: newPosition.x = std::max(0, position.x - steps); break;
        }
        setPosition(newPosition);

        // Choose a new random direction that's not blocked
        Direction originalDirection = direction;
        do {
            direction = static_cast<Direction>(rand() % 4 + 1);
        } while (direction == originalDirection || isWayBlocked());
    }
}

std::string Hopper::toString() const {
    std::ostringstream oss;
    oss << id << " " << getType() << " (" << position.x << "," << position.y << ") " 
        << size << " " << getDirectionAsString() << " HopLength:" << hopLength << " "
        << (alive ? "Alive" : "Dead");
    return oss.str();
}