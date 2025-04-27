#ifndef ROLLER_H
#define ROLLER_H

#include "Bug.h"

class Roller : public Bug {
public:
    Roller(int id, int x, int y, Direction direction, int size);

    void move() override;
    std::string getType() const override { return "Roller"; }
};

#endif