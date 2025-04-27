#include "Board.h"
#include "Crawler.h"
#include "Hopper.h"
#include "Roller.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <chrono>
#include <thread>
#include <ctime>
#include <iomanip>

Board::Board() {
    srand(time(0)); //random generator
}

Board::~Board() {
    for (Bug* bug : bugs) {
        delete bug;
    }
}

void Board::initializeBugBoard(const std::string& filename) { //Initialize the board
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    bugs.clear(); // Clear existing bugs
    std::string line;
    while (getline(file, line)) { //reading data from file
        std::istringstream iss(line);
        char type;
        int id, x, y, dir, size, hopLength = 0;
        char comma;

        iss >> type >> comma;
        if (type == 'C') {
            iss >> id >> comma >> x >> comma >> y >> comma >> dir >> comma >> size;
            Direction direction = static_cast<Direction>(dir);
            bugs.push_back(new Crawler(id, x, y, direction, size));
        } else if (type == 'H') {
            iss >> id >> comma >> x >> comma >> y >> comma >> dir >> comma >> size >> comma >> hopLength;
            Direction direction = static_cast<Direction>(dir);
            bugs.push_back(new Hopper(id, x, y, direction, size, hopLength));
        } else if (type == 'R') {
            iss >> id >> comma >> x >> comma >> y >> comma >> dir >> comma >> size;
            Direction direction = static_cast<Direction>(dir);
            bugs.push_back(new Roller(id, x, y, direction, size));
        }
    }

    file.close();
    updateCellOccupancy();
    std::cout << "Bug board initialized with " << bugs.size() << " bugs." << std::endl;
}

void Board::displayAllBugs() const {
    if (bugs.empty()) {
        std::cout << "No bugs on the board. Please initialize the board first." << std::endl;
        return;
    }

    std::cout << "All Bugs:" << std::endl;
    std::cout << "--------------------------------------" << std::endl;
    for (const Bug* bug : bugs) {
        std::cout << bug->toString() << std::endl;
    }
    std::cout << "----------------------------------------" << std::endl;
}

void Board::findBug(int id) const { //method for finding the bug
    auto it = std::find_if(bugs.begin(), bugs.end(),
        [id](const Bug* bug) { return bug->getId() == id; });

    if (it != bugs.end()) {
        std::cout << "Bug found:" << std::endl;
        std::cout << (*it)->toString() << std::endl;
    } else {
        std::cout << "Bug " << id << " not found." << std::endl;
    }
}

void Board::tapBugBoard() { //make 1 iteration
    if (bugs.empty()) {
        std::cout << "No bugs on the board. Please initialize the board first." << std::endl;
        return;
    }

    for (Bug* bug : bugs) {
        if (bug->isAlive()) {
            bug->move();
        }
    }

    updateCellOccupancy();

    for (auto& cell : cellOccupancy) { //map
        if (cell.second.size() > 1) {
            fightInCell(cell.first);
        }
    }

    std::cout << "Board tapped. " << getAliveBugCount() << " bugs remaining." << std::endl;
}

void Board::updateCellOccupancy() {
    cellOccupancy.clear();

    for (Bug* bug : bugs) {
        if (bug->isAlive()) {
            Position pos = bug->getPosition();
            cellOccupancy[{pos.x, pos.y}].push_back(bug);
        }
    }
}

void Board::fightInCell(std::pair<int, int> cellPosition) {
    auto& bugsInCell = cellOccupancy[cellPosition]; //check and apply cell occupancy

    std::sort(bugsInCell.begin(), bugsInCell.end(),
        [](const Bug* a, const Bug* b) { return a->getSize() > b->getSize(); }); //sorting bugs in cell

    int maxSize = bugsInCell[0]->getSize(); //choosing winner by size
    std::vector<Bug*> winners;

    for (Bug* bug : bugsInCell) {
        if (bug->getSize() == maxSize) {
            winners.push_back(bug);
        } else {
            break;
        }
    }

    Bug* winner = winners[rand() % winners.size()];
    int totalSizeIncrease = 0;

    for (Bug* bug : bugsInCell) {
        if (bug != winner && bug->isAlive()) {
            bug->setAlive(false);
            totalSizeIncrease += bug->getSize();
        }
    }

    winner->setSize(winner->getSize() + totalSizeIncrease);

    std::cout << "Fight at (" << cellPosition.first << "," << cellPosition.second << "): "
              << winner->getType() << " " << winner->getId() << " wins and grows to size "
              << winner->getSize() << std::endl;
}

void Board::displayLifeHistory() const {
    if (bugs.empty()) {
        std::cout << "No bugs on the board. Please initialize the board first." << std::endl;
        return;
    }

    std::cout << "Life History of All Bugs:" << std::endl;
    std::cout << "----------------------------------------" << std::endl;

    for (const Bug* bug : bugs) {
        std::cout << bug->getId() << " " << bug->getType() << " Path: ";

        const auto& path = bug->getPath();
        for (auto it = path.begin(); it != path.end(); ++it) {
            std::cout << "(" << it->x << "," << it->y << ")";
            if (std::next(it) != path.end()) {
                std::cout << ",";
            }
        }

        if (!bug->isAlive()) {
            for (const Bug* other : bugs) {
                if (other->isAlive() && other->getPosition() == path.back()) {
                    std::cout << " Eaten by " << other->getId();
                    break;
                }
            }
        } else {
            std::cout << " Alive";
        }

        std::cout << std::endl;
    }
    std::cout << "----------------------------------------" << std::endl;
}

void Board::displayAllCells() const {
    std::cout << "All Cells and Their Bugs:" << std::endl;
    std::cout << "----------------------------------------" << std::endl;

    for (int y = 0; y < 10; ++y) {
        for (int x = 0; x < 10; ++x) {
            auto it = cellOccupancy.find({x, y});
            std::cout << "(" << x << "," << y << "): ";

            if (it == cellOccupancy.end() || it->second.empty()) {
                std::cout << "empty";
            } else {
                for (size_t i = 0; i < it->second.size(); ++i) {
                    std::cout << it->second[i]->getType() << " " << it->second[i]->getId();
                    if (i < it->second.size() - 1) {
                        std::cout << ", ";
                    }
                }
            }

            std::cout << std::endl;
        }
    }
    std::cout << "----------------------------------------" << std::endl;
}

void Board::runSimulation() {
    if (bugs.empty()) {
        std::cout << "No bugs on the board. Please initialize the board first." << std::endl;
        return;
    }

    std::cout << "Starting simulation..." << std::endl;
    int tapCount = 0;

    while (!isGameOver()) {
        tapBugBoard();
        tapCount++;
        std::cout << "After tap " << tapCount << ": " << getAliveBugCount()
                  << " bugs remaining." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    Bug* winner = nullptr;
    for (Bug* bug : bugs) {
        if (bug->isAlive()) {
            winner = bug;
            break;
        }
    }

    if (winner) {
        std::cout << "Simulation complete! Winner: " << winner->getType() << " "
                  << winner->getId() << " with size " << winner->getSize() << std::endl;
    }

    writeLifeHistoryToFile();
}

void Board::writeLifeHistoryToFile() const {
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::tm* now_tm = std::localtime(&now_time);

    std::ostringstream filename;
    filename << "bugs_life_history_"
             << std::put_time(now_tm, "%Y%m%d_%H%M%S") << ".out";

    std::ofstream outFile(filename.str());
    if (!outFile.is_open()) {
        std::cerr << "Error creating output file." << std::endl;
        return;
    }

    for (const Bug* bug : bugs) {
        outFile << bug->getId() << " " << bug->getType() << " Path: ";

        const auto& path = bug->getPath();
        for (auto it = path.begin(); it != path.end(); ++it) {
            outFile << "(" << it->x << "," << it->y << ")";
            if (std::next(it) != path.end()) {
                outFile << ",";
            }
        }

        if (!bug->isAlive()) {
            for (const Bug* other : bugs) {
                if (other->isAlive() && other->getPosition() == path.back()) {
                    outFile << " Eaten by " << other->getId();
                    break;
                }
            }
        } else {
            outFile << " Alive";
        }

        outFile << std::endl;
    }

    outFile.close();
    std::cout << "Life history written to file: " << filename.str() << std::endl;
}

bool Board::isGameOver() const {
    return getAliveBugCount() <= 1;
}

int Board::getAliveBugCount() const {
    int count = 0;
    for (const Bug* bug : bugs) {
        if (bug->isAlive()) count++;
    }
    return count;
}