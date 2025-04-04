#include "Board.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <chrono>
#include <thread>
#include <ctime>
#include <iomanip>

Board::Board() {
    srand(time(0));
}

Board::~Board() {
    for (Crawler* crawler : crawlers) {
        delete crawler;
    }
}

void Board::initializeBugBoard(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }
    
    std::string line;
    while (getline(file, line)) {
        std::istringstream iss(line);
        char type;
        int id, x, y, dir, size;
        char comma;
        
        iss >> type >> comma 
            >> id >> comma 
            >> x >> comma 
            >> y >> comma 
            >> dir >> comma 
            >> size;
            
        if (type == 'C') {
            Direction direction = static_cast<Direction>(dir);
            Crawler* crawler = new Crawler(id, x, y, direction, size);
            crawlers.push_back(crawler);
        }
    }
    
    file.close();
    updateCellOccupancy();
    std::cout << "Bug board initialized with " << crawlers.size() << " crawlers." << std::endl;
}

void Board::displayAllBugs() const {
    if (crawlers.empty()) {
        std::cout << "No bugs on the board. Please initialize the board first." << std::endl;
        return;
    }
    
    std::cout << "All Bugs:" << std::endl;
    std::cout << "--------------------------------------" << std::endl;
    for (const Crawler* crawler : crawlers) {
        std::cout << crawler->toString() << std::endl;
    }
    std::cout << "----------------------------------------" << std::endl;
}

void Board::findBug(int id) const {
    auto it = std::find_if(crawlers.begin(), crawlers.end(), 
        [id](const Crawler* crawler) { return crawler->getId() == id; });
    
    if (it != crawlers.end()) {
        std::cout << "Bug found:" << std::endl;
        std::cout << (*it)->toString() << std::endl;
    } else {
        std::cout << "Bug " << id << " not found." << std::endl;
    }
}

void Board::tapBugBoard() {
    if (crawlers.empty()) {
        std::cout << "No bugs on the board. Please initialize the board first." << std::endl;
        return;
    }
    
    // Move all alive bugs
    for (Crawler* crawler : crawlers) {
        if (crawler->isAlive()) {
            crawler->move();
        }
    }

    updateCellOccupancy();
    
    // Check for fights in all cells
    for (auto& cell : cellOccupancy) {
        if (cell.second.size() > 1) {
            fightInCell(cell.first);
        }
    }
    
    std::cout << "Board tapped. " << getAliveBugCount() << " bugs remaining." << std::endl;
}

void Board::updateCellOccupancy() {
    cellOccupancy.clear();
    
    for (Crawler* crawler : crawlers) {
        if (crawler->isAlive()) {
            Position pos = crawler->getPosition();
            cellOccupancy[{pos.x, pos.y}].push_back(crawler);
        }
    }
}

void Board::fightInCell(std::pair<int, int> cellPosition) {
    auto& bugsInCell = cellOccupancy[cellPosition];

    std::sort(bugsInCell.begin(), bugsInCell.end(), 
        [](const Crawler* a, const Crawler* b) { return a->getSize() > b->getSize(); });

    int maxSize = bugsInCell[0]->getSize();
    std::vector<Crawler*> winners;
    
    for (Crawler* bug : bugsInCell) {
        if (bug->getSize() == maxSize) {
            winners.push_back(bug);
        } else {
            break;
        }
    }

    Crawler* winner = winners[rand() % winners.size()];
    int totalSizeIncrease = 0;

    for (Crawler* bug : bugsInCell) {
        if (bug != winner && bug->isAlive()) {
            bug->setAlive(false);
            totalSizeIncrease += bug->getSize();
        }
    }

    winner->setSize(winner->getSize() + totalSizeIncrease);
    
    std::cout << "Fight at (" << cellPosition.first << "," << cellPosition.second << "): "
              << "Bug " << winner->getId() << " wins and grows to size " 
              << winner->getSize() << std::endl;
}

void Board::displayLifeHistory() const {
    if (crawlers.empty()) {
        std::cout << "No bugs on the board. Please initialize the board first." << std::endl;
        return;
    }
    
    std::cout << "Life History of All Bugs:" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    
    for (const Crawler* crawler : crawlers) {
        std::cout << crawler->getId() << " Crawler Path: ";
        
        const auto& path = crawler->getPath();
        for (auto it = path.begin(); it != path.end(); ++it) {
            std::cout << "(" << it->x << "," << it->y << ")";
            if (std::next(it) != path.end()) {
                std::cout << ",";
            }
        }
        
        if (!crawler->isAlive()) {
            for (const Crawler* other : crawlers) {
                if (other->isAlive()) {
                    Position lastPos = path.back();
                    if (other->getPosition().x == lastPos.x && other->getPosition().y == lastPos.y) {
                        std::cout << " Eaten by " << other->getId();
                        break;
                    }
                }
            }
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
                    std::cout << "Crawler " << it->second[i]->getId();
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
    if (crawlers.empty()) {
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

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    Crawler* winner = nullptr;
    for (Crawler* crawler : crawlers) {
        if (crawler->isAlive()) {
            winner = crawler;
            break;
        }
    }
    
    if (winner) {
        std::cout << "Simulation complete! Winner: Bug " << winner->getId() 
                  << " with size " << winner->getSize() << std::endl;
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
    
    for (const Crawler* crawler : crawlers) {
        outFile << crawler->getId() << " Crawler Path: ";
        
        const auto& path = crawler->getPath();
        for (auto it = path.begin(); it != path.end(); ++it) {
            outFile << "(" << it->x << "," << it->y << ")";
            if (std::next(it) != path.end()) {
                outFile << ",";
            }
        }
        
        if (!crawler->isAlive()) {
            for (const Crawler* other : crawlers) {
                if (other->isAlive()) {
                    Position lastPos = path.back();
                    if (other->getPosition().x == lastPos.x && other->getPosition().y == lastPos.y) {
                        outFile << " Eaten by " << other->getId();
                        break;
                    }
                }
            }
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
    for (const Crawler* crawler : crawlers) {
        if (crawler->isAlive()) count++;
    }
    return count;
}