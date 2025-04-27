#ifndef BOARD_H
#define BOARD_H

#include "Bug.h"
#include <vector>
#include <string>
#include <map>

class Board {
private:
    std::vector<Bug*> bugs;
    std::map<std::pair<int, int>, std::vector<Bug*>> cellOccupancy;
    
    void updateCellOccupancy();
    void fightInCell(std::pair<int, int> cellPosition);
    
public:
    Board();
    ~Board();
    
    void initializeBugBoard(const std::string& filename);
    void displayAllBugs() const;
    void findBug(int id) const;
    void tapBugBoard();
    void displayLifeHistory() const;
    void displayAllCells() const;
    void runSimulation();
    void writeLifeHistoryToFile() const;
    
    bool isGameOver() const;
    int getAliveBugCount() const;
};

#endif