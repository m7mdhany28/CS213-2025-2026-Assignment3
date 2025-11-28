#include "MainScreen.h"
#include "PlayGameFunctions.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <limits>

using namespace std;

void MainScreen::printMainMenuScreen() {
    cout << "\n" << string(50, '=') << "\n";
    cout << "           MAIN GAME SELECTION MENU\n";
    cout << string(50, '=') << "\n";
    cout << "1. Misere Tic Tac Toe (Reverse Rules) - Lose to win!\n";
    cout << "2. 5x5 Tic Tac Toe (Expanded Board) - Think bigger!\n";
    cout << "3. Word Tic Tac Toe (Vocabulary Challenge) - Spell victory!\n";
    cout << "4. Infinity Tic-Tac-Toe (Temporal Challenge) - Race against time!\n";
    cout << "5. 4x4 Moving Tic-Tac-Toe (Token Movement) - Keep moving!\n";
    cout << "6. Pyramid Tic-Tac-Toe (Pyramid Board) - Build upward!\n";
    cout << "7. Exit Game Collection - Until next time!\n";
    cout << string(50, '=') << "\n";
    cout << "What gaming adventure calls to you? (1-7): ";
}

void MainScreen::implementUserChoice(UserChoice choice) {
    switch (choice) {
    case UserChoice::MISERE:
        PlayGameFunctions::playMisereGame();
        break;
    case UserChoice::FIVE_BY_FIVE:
        PlayGameFunctions::play5x5Game();
        break;
    case UserChoice::WORD:
        PlayGameFunctions::playWordGame();
        break;
    case UserChoice::INFINITY_GAME:
        PlayGameFunctions::playInfinityGame();
        break;
    case UserChoice::MOVING:
        PlayGameFunctions::playMovingGame();
        break;
    case UserChoice::PYRAMID:
        PlayGameFunctions::playPyramidGame();
        break;
    case UserChoice::EXIT:
        cout << "\nThank you for visiting our Board Games Collection!\n";
        break;
    }
}

void MainScreen::startBoardGameProgram() {
    srand(static_cast<unsigned int>(time(0)));

    cout << "======================================================\n";
    cout << "           WELCOME TO BOARD GAMES COLLECTION\n";
    cout << "======================================================\n";

    while (true) {
        printMainMenuScreen();

        int choice;
        while (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Let's try that again - enter a number between 1 and 7: ";
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice < 1 || choice > 7) {
            cout << "Please pick a number between 1 and 7 from our menu.\n";
            continue;
        }

        UserChoice userChoice = static_cast<UserChoice>(choice);

        if (userChoice == UserChoice::EXIT) {
            implementUserChoice(userChoice);
            break;
        }

        implementUserChoice(userChoice);

        cout << "\nPress Enter when you're ready to see the main menu again...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
    }
}