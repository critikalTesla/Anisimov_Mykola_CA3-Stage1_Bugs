#include "Crawler.h"
#include <cstdlib>
#include <ctime>

Crawler::Crawler(int id, int x, int y, Direction direction, int size)
    : Bug(id, x, y, direction, size) {
    srand(time(0)); // Initialize random seed
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
        setPosition(newPosition);
        } else {
            Direction originalDirection = direction;
            do {
                direction = static_cast<Direction>(rand() % 4 + 1);
            } while (direction == originalDirection || isWayBlocked());
            move();
        }
}