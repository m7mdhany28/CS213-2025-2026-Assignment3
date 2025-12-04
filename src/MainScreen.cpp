#include "MainScreen.h"
#include "PlayGameFunctions.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <cctype>

using namespace std;

void MainScreen::printMainMenuScreen() {
    cout << "\n" << string(50, '=') << "\n";
    cout << "           BOARD GAMES COLLECTION - AI EDITION\n";
    cout << string(50, '=') << "\n";
    cout << "1. XO Game (Classic Tic Tac Toe)\n";
    cout << "2. Numerical Tic Tac Toe (Math Challenge)\n";
    cout << "3. SUS Game (S-U-S Sequence)\n";
    cout << "4. Four In Row Game (Connect Four)\n";
    cout << "5. Infinity Tic-Tac-Toe (Temporal Challenge)\n";
    cout << "6. Misere Tic Tac Toe (Reverse Rules) - Lose to win!\n";
    cout << "7. 5x5 Tic Tac Toe (Expanded Board) - Think bigger!\n";
    cout << "8. Word Tic Tac Toe (Vocabulary Challenge) - Spell victory!\n";
    cout << "9. 4x4 Moving Tic-Tac-Toe (Token Movement) - Keep moving!\n";
    cout << "10. Pyramid Tic-Tac-Toe (Pyramid Board) - Build upward!\n";
    cout << "11. XO Memory Game (Memory Challenge)\n";
    cout << "12. Exit Game Collection - Until next time!\n";
    cout << string(50, '=') << "\n";
    cout << "What gaming adventure calls to you? (1-12): ";
}

void MainScreen::implementUserChoice(UserChoice choice) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    switch (choice) {
    case UserChoice::XOGAME:
        PlayGameFunctions::playXOGame();
        break;
    case UserChoice::NUMERICALGAME:
        PlayGameFunctions::playNumericalGame();
        break;
    case UserChoice::SUSGAME:
        PlayGameFunctions::playSUSGame();
        break;
    case UserChoice::FOURINROWGAME:
        PlayGameFunctions::playFourInRowGame();
        break;
    case UserChoice::INFINITYGAME:
        PlayGameFunctions::playInfinityGame();
        break;
    case UserChoice::MISERE:
        PlayGameFunctions::playMisereGame();
        break;
    case UserChoice::FIVE_BY_FIVE:
        PlayGameFunctions::play5x5Game();
        break;
    case UserChoice::WORD:
        PlayGameFunctions::playWordGame();
        break;
    case UserChoice::MOVING:
        PlayGameFunctions::playMovingGame();
        break;
    case UserChoice::PYRAMID:
        PlayGameFunctions::playPyramidGame();
        break;
    case UserChoice::MEMORYGAME:
        PlayGameFunctions::playMemoryGame();
        break;
    case UserChoice::EXIT:
        cout << "\nThank you for visiting our Board Games Collection!\n";
        break;
    }
}

void MainScreen::startBoardGameProgram() {
    srand(static_cast<unsigned int>(time(0)));

    cout << "======================================================\n";
    cout << "        BOARD GAMES COLLECTION - AI EDITION\n";
    cout << "======================================================\n";
    cout << "======================================================\n";

    char playAgain = 'Y'; // FIXED: Initialize variable

    do {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif

        printMainMenuScreen();

        int choice;
        while (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Let's try that again - enter a number between 1 and 12: ";
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice < 1 || choice > 12) {
            cout << "Please pick a number between 1 and 12 from our menu.\n";
            continue;
        }

        UserChoice userChoice = static_cast<UserChoice>(choice);

        if (userChoice == UserChoice::EXIT) {
            implementUserChoice(userChoice);
            break;
        }

        implementUserChoice(userChoice);

        cout << "\nGood Game!\n";
        cout << "Do you want to play again? [Y/N]: ";
        cin >> playAgain;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

    } while (toupper(playAgain) == 'Y'); // FIXED: No comma operator

    cout << "\nGood Bye!\n";
}