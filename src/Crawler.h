#ifndef CRAWLER_H
#define CRAWLER_H

#include "Position.h"
#include <list>
#include <string>
#include "Bug.h"

class Crawler : public Bug {
public:
    Crawler(int id, int x, int y, Direction direction, int size);
    void move() override;
    std::string getType() const override { return "Crawler"; }
};

#endif