#include "Board.h"
#include <iostream>
#include <limits>

void displayMenu() {
    std::cout << "\nBug's Life Simulation Menu:" << std::endl;
    std::cout << "1. Initialize Bug Board (load data from file)" << std::endl;
    std::cout << "2. Display all 'Bugs'" << std::endl;
    std::cout << "3. Find a 'Bug' (by id)" << std::endl;
    std::cout << "4. Tap the Bug Board (cause all to move, then fight/eat)" << std::endl;
    std::cout << "5. Display Life History of all 'Bugs' path" << std::endl;
    std::cout << "6. Display all Cells listing their 'Bugs'" << std::endl;
    std::cout << "7. Run simulation " << std::endl;
    std::cout << "8. Exit" << std::endl;
    std::cout << "Enter your choice (1-8): ";
}

int getIntegerInput(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            break;
        } else {
            std::cout << "Invalid input. Please enter an integer." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    return value;
}

int main() {
    Board bugBoard;
    int choice;
    bool exitProgram = false;

    while (!exitProgram) {
        displayMenu();
        choice = getIntegerInput("");

        switch (choice) {
            case 1: {
                std::string filename = "crawler-bugs.csv";
                bugBoard.initializeBugBoard(filename);
                break;
            }
            case 2:
                bugBoard.displayAllBugs();
                break;
            case 3: {
                int id = getIntegerInput("Enter bug ID to find: ");
                bugBoard.findBug(id);
                break;
            }
            case 4:
                bugBoard.tapBugBoard();
                break;
            case 5:
                bugBoard.displayLifeHistory();
                break;
            case 6:
                bugBoard.displayAllCells();
                break;
            case 7:
                bugBoard.runSimulation();
                break;
            case 8:
                bugBoard.writeLifeHistoryToFile();
                exitProgram = true;
                std::cout << "Exiting program. Goodbye!" << std::endl;
                break;
            default:
                std::cout << "Invalid choice. Please enter a number between 1 and 8." << std::endl;
        }
    }

    return 0;
}